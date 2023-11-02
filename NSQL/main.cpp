#include "sock.h"

int main(void)
{
    Socket* s = new Socket();
    while (1) {
        s->startListen();
    }
    return 0;
}