static void prepare_poll_fds(int h, struct pollfd* pfds)
{
 int count = 0;
 int ps_i = 0;
 int pfd_i = 0;
    asrt(ts[h].poll_count <= MAX_POLL);
    memset(pfds, 0, sizeof(pfds[0])*ts[h].poll_count);
 while(count < ts[h].poll_count)
 {
 if(ps_i >= MAX_POLL)
 {
            APPL_TRACE_ERROR("exceed max poll range, ps_i:%d, MAX_POLL:%d, count:%d, ts[h].poll_count:%d",
                    ps_i, MAX_POLL, count, ts[h].poll_count);
 return;
 }
 if(ts[h].ps[ps_i].pfd.fd >= 0)
 {
            pfds[pfd_i] =  ts[h].ps[ps_i].pfd;
            ts[h].psi[pfd_i] = ps_i;
            count++;
            pfd_i++;
 }
        ps_i++;
 }
}
