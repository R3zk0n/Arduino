

#include "Mach_Vouch.h"
#include <errno.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mach/mach.h>
#include <mach-o/loader.h>
#include <dlfcn.h>

static void hexdump(const void* data, size_t size) {
    char ascii[17];
    size_t i, j;
    ascii[16] = '\0';
    for (i = 0; i < size; ++i) {
        printf("%02X ", ((unsigned char*)data)[i]);
        if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
            ascii[i % 16] = ((unsigned char*)data)[i];
        } else {
            ascii[i % 16] = '.';
        }
        if ((i+1) % 8 == 0 || i+1 == size) {
            printf(" ");
            if ((i+1) % 16 == 0) {
                printf("|  %s \n", ascii);
            } else if (i+1 == size) {
                ascii[(i+1) % 16] = '\0';
                if ((i+1) % 16 <= 8) {
                    printf(" ");
                }
                for (j = (i+1) % 16; j < 16; ++j) {
                    printf("   ");
                }
                printf("|  %s \n", ascii);
            }
        }
    }
}


int main( int argc, const char* argv[] ){
	mach_port_t port, bp;
	kern_return_t ret = task_get_bootstrap_port(mach_task_self(), &bp);
	port = bp;
    printf("Replying XPC Call\n");
	unsigned char payload[] = {};
    hexdump(payload, sizeof(payload));
	mach_msg_header_t *msg = (mach_msg_header_t *)payload;
	msg->msgh_remote_port = port;
	msg->msgh_local_port = MACH_PORT_NULL;
	msg->msgh_bits = MACH_MSGH_BITS_ZERO;
	msg->msgh_bits = MACH_MSGH_BITS_COMPLEX | MACH_MSGH_BITS_SET_PORTS(MACH_MSG_TYPE_COPY_SEND, MACH_MSG_TYPE_MAKE_SEND_ONCE, MACH_MSG_TYPE_COPY_SEND);
	mach_msg_return_t msg_ret = mach_msg_send(msg);
    printf("Send XPC Call\n");
    printf("Msg Ret: 0x%x", msg_ret);
}
