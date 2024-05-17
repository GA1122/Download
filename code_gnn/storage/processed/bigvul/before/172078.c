static inline unsigned int flags2pevents(int flags)
{
 unsigned int pevents = 0;
 if(flags & SOCK_THREAD_FD_WR)
        pevents |= POLLOUT;
 if(flags & SOCK_THREAD_FD_RD)
        pevents |= POLLIN;
    pevents |= POLL_EXCEPTION_EVENTS;
 return pevents;
}
