#include "newtork_initializer.h"

NetworkInitializer g_network_initializer;

bool setSocketNoBlock(int fd)
{
    int ret = 0;
#ifdef __linux__
    int flags;
#if defined(O_NONBLOCK)
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    ret = ioctl(fd, FIOBIO, &flags);
#endif
#endif

#ifdef _WIN32
    unsigned long mode_ = 1;
    ret = ioctlsocket(fd, FIONBIO, &mode_);
#endif
    if (ret >= 0) return true;
    return false;
}