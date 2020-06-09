#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

void recvDownlink(void*);
void sendUplink(void*);

const long POLL_TIMEOUT_MILLISEC = 1000; //1 second

int main() {
    //  create ipc socket for publishing uplink from GS
    void *context = zmq_ctx_new();
    void *zuplink = zmq_socket(context, ZMQ_PUB);
    int rc = zmq_bind(zuplink, "ipc://uplink");
    assert(rc == 0);

    // create ipc socket for subscribing to downlink from sat

    void *zdownlink = zmq_socket(context, ZMQ_SUB);
    rc = zmq_connect(zdownlink, "tcp://localhost:5554");
    assert(rc==0);



    zmq_pollitem_t items[1];
    items[0].socket = zdownlink;
    items[0].events = ZMQ_POLLIN;


    while(1) {
      // block for 1 sec to see if something can be read from downlink
      rc = zmq_poll(items, 1, POLL_TIMEOUT_MILLISEC);
      if (rc > 0 ) {
        printf("Done Poll\n");
        if (items[0].revents & ZMQ_POLLIN) {
          //ready to read downlink message
          printf("reading downlink msg\n");
          recvDownlink(zdownlink);
        }
      }
      else if (rc == 0) {
        printf("poll did not return any\n");
      }
      else if (rc == -1){
        printf("Poll failed: %s\n", strerror(errno));
      }
      // send uplink command here
      sendUplink(zuplink);

/*
      sendUplink(zuplink);
      sleep(1);
*/
    }
    return 0;
}

void recvDownlink(void* zsock){
  char buf [128];
  int nbytes = zmq_recv (zsock, buf, 128, 0); assert (nbytes != -1);
  buf[nbytes] = '\0'; // null terminate string on arrival
  printf("Ground Station got downlinked message %s\n", buf);
}

void sendUplink(void* zsock){
  char msg[] = "{Message:'Hello From GoundStation', TimeStamp:1234567}";
  printf("%zu", sizeof(msg));
  int rc = zmq_send(zsock, msg, sizeof(msg), 0);
  if (rc == -1){
    printf("Message failed to uplink: %s\n", strerror(errno));
  }
  else{
    printf("Uplink success. %d bytes written\n", rc);
  }
}
