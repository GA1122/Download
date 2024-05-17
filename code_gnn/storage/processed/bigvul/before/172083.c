static void process_data_sock(int h, struct pollfd *pfds, int count)
{
    asrt(count <= ts[h].poll_count);
 int i;
 for( i= 1; i < ts[h].poll_count; i++)
 {
 if(pfds[i].revents)
 {
 int ps_i = ts[h].psi[i];
            asrt(pfds[i].fd == ts[h].ps[ps_i].pfd.fd);
 uint32_t user_id = ts[h].ps[ps_i].user_id;
 int type = ts[h].ps[ps_i].type;
 int flags = 0;
            print_events(pfds[i].revents);
 if(IS_READ(pfds[i].revents))
 {
                flags |= SOCK_THREAD_FD_RD;
 }
 if(IS_WRITE(pfds[i].revents))
 {
                flags |= SOCK_THREAD_FD_WR;
 }
 if(IS_EXCEPTION(pfds[i].revents))
 {
                flags |= SOCK_THREAD_FD_EXCEPTION;
                remove_poll(h, &ts[h].ps[ps_i], ts[h].ps[ps_i].flags);
 }
 else if(flags)
                 remove_poll(h, &ts[h].ps[ps_i], flags);  
 if(flags)
                ts[h].callback(pfds[i].fd, type, flags, user_id);
 }
 }
}
