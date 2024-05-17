static inline void add_poll(int h, int fd, int type, int flags, uint32_t user_id)
{
    asrt(fd != -1);
 int i;
 int empty = -1;
 poll_slot_t* ps = ts[h].ps;

 for(i = 0; i < MAX_POLL; i++)
 {
 if(ps[i].pfd.fd == fd)
 {
            asrt(ts[h].poll_count < MAX_POLL);

            set_poll(&ps[i], fd, type, flags | ps[i].flags, user_id);
 return;
 }
 else if(empty < 0 && ps[i].pfd.fd == -1)
            empty = i;
 }
 if(empty >= 0)
 {
        asrt(ts[h].poll_count < MAX_POLL);
        set_poll(&ps[empty], fd, type, flags, user_id);
 ++ts[h].poll_count;
 return;
 }
    APPL_TRACE_ERROR("exceeded max poll slot:%d!", MAX_POLL);
}
