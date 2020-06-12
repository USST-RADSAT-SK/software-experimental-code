#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "message.h"

void recvUplink(void*);
void sendDownlink(void*);

const long POLL_TIMEOUT_MILLISEC = 1000; //1 second

int main(void){
  //  create ipc socket for subscribing to uplink from GS
  void *context = zmq_ctx_new();
  void *zuplink = zmq_socket(context, ZMQ_PULL);
  int rc = zmq_connect(zuplink, "ipc://uplink");
  assert(rc == 0);

  // create ipc socket for publishing to downlink from sat
  void *zdownlink = zmq_socket(context, ZMQ_PUSH);
  rc = zmq_bind(zdownlink, "ipc://downlink");
  assert(rc==0);

  zmq_pollitem_t items[1];
  items[0].socket = zuplink;
  items[0].events = ZMQ_POLLIN;


  while(1) {
    // block for 1 sec to see if something can be read from uplink
    rc = zmq_poll(items, 1, POLL_TIMEOUT_MILLISEC);
    if (rc > 0 ){
      if (items[0].revents & ZMQ_POLLIN) {
        //ready to read uplink message
        recvUplink(zuplink);
      }
    }
    // send downlink command here
    sleep(1);
    sendDownlink(zdownlink);
  }
  return 0;
}

void recvUplink(void* zsock){
  message_t msg;
  int nbytes = zmq_recv (zsock, &msg, sizeof(msg), 0); assert (nbytes != -1); // recv directly into message struct
  printf("Sat got uplinked message \"%s\" with time stamp %d\n", msg.string, msg.timeStamp);
}


void sendDownlink(void* zsock){
  message_t msg = {"Hello GS. This is downlink", 456325};
  int rc = zmq_send(zsock, &msg, sizeof(msg), 0);
  if (rc == -1) {
    printf("Message failed to downlink: %s\n", strerror(errno));
  }
}
