#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "message.h"

void recvDownlink(void*);
void sendUplink(void*);

const long POLL_TIMEOUT_MILLISEC = 1000; //1 second

int main() {
    //  create ipc socket for publishing uplink from GS
    void *context = zmq_ctx_new();
    void *zuplink = zmq_socket(context, ZMQ_PUSH);
    int rc = zmq_bind(zuplink, "ipc://uplink");
    assert(rc == 0);

    // create ipc socket for subscribing to downlink from sat
    void *zdownlink = zmq_socket(context, ZMQ_PULL);
    rc = zmq_connect(zdownlink, "ipc://downlink");
    assert(rc==0);

    zmq_pollitem_t items[1];
    items[0].socket = zdownlink;
    items[0].events = ZMQ_POLLIN;

    while(1) {
      // block for 1 sec to see if something can be read from downlink
      rc = zmq_poll(items, 1, POLL_TIMEOUT_MILLISEC);
      if (rc > 0 ) {
        if (items[0].revents & ZMQ_POLLIN) {
          //ready to read downlink message
          recvDownlink(zdownlink);
        }
      }
      // send uplink command here
      sendUplink(zuplink);
    }
    return 0;
}

void recvDownlink(void* zsock){
  message_t msg;
  int nbytes = zmq_recv (zsock, &msg, sizeof(msg), 0); assert (nbytes != -1); // recv directly into message struct
  printf("Ground Station got downlinked message \"%s\" with time stamp %d\n", msg.string, msg.timeStamp);
}

void sendUplink(void* zsock){
  message_t msg = {"Hello Sat. This is uplink", 457862};
  int rc = zmq_send(zsock, &msg, sizeof(msg), 0);
  if (rc == -1){
    printf("Message failed to uplink: %s\n", strerror(errno));
  }
}
