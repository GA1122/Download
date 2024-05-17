static inline void remove_poll(int h, poll_slot_t* ps, int flags)
{
 if(flags == ps->flags)
 {
 --ts[h].poll_count;
        memset(ps, 0, sizeof(*ps));
        ps->pfd.fd = -1;
 }
 else
 {
        ps->flags &= ~flags;
        ps->pfd.events = flags2pevents(ps->flags);
 }
}
