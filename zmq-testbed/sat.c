#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>


void recvUplink(void*);
void sendDownlink(void*);

const long POLL_TIMEOUT_MILLISEC = 1000; //1 second

int main(void){
  //  create ipc socket for subscribing to uplink from GS
  void *context = zmq_ctx_new();
  void *zuplink = zmq_socket(context, ZMQ_SUB);
  int rc = zmq_connect(zuplink, "ipc://uplink");
  assert(rc == 0);


/*
  // create ipc socket for publishing to downlink from sat
  void *zdownlink = zmq_socket(context, ZMQ_PUB);
  rc = zmq_bind(zdownlink, "tcp://*:5554");
  assert(rc==0);
*/


  printf("Sockets created\n");

  /*
  zmq_pollitem_t items[1];
  items[0].socket = zuplink;
  items[0].events = ZMQ_POLLIN;
  */


  while(1) {
    // block for 1 sec to see if something can be read from uplink

    recvUplink(zuplink);

    /*
    rc = zmq_poll(items, 1, POLL_TIMEOUT_MILLISEC);
    if (rc > 0 ){
      printf("Done Poll\n");
      if (items[0].revents & ZMQ_POLLIN) {
        //ready to read uplink message
        printf("reading uplink msg\n");
        recvUplink(zuplink);
      }
    }
    else if(rc == 0){
      printf("Poll did not return any\n");
    }
    else if (rc == -1){
      printf("Poll failed: %s\n", strerror(errno));
    }
    // send downlink command here
    sendDownlink(zdownlink);
    */
    
  }
  return 0;
}

void recvUplink(void* zsock){
  printf("recieving\n");
  char buf [256];
  int nbytes = zmq_recv (zsock, buf, 256, 0);
  if (nbytes != -1){
    buf[nbytes] = '\0'; // null terminate string on arrival
    printf("Satellite got uplinked message %s\n", buf);
  }
}


void sendDownlink(void* zsock){
  char msg[] = "{Message:'Hello From Sat', TimeStamp:1234567}";
  int rc = zmq_send(zsock, msg, sizeof(msg), 0);
  if (rc == -1) {
    printf("Message failed to downlink: %s\n", strerror(errno));
  }
  else{
    printf("downlink success: sent %d bytes\n", rc);
  }
}
