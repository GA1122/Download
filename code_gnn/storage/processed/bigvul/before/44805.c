local size_t load(void)
{
     
    if (g.in_short) {
        g.in_eof = 1;
        g.in_left = 0;
        return 0;
    }

#ifndef NOTHREAD
     
    if (g.procs > 1) {
         
        if (g.in_which == -1) {
            g.in_which = 1;
            g.load_state = new_lock(1);
            g.load_thread = launch(load_read, NULL);
        }

         
        possess(g.load_state);
        wait_for(g.load_state, TO_BE, 0);
        release(g.load_state);

         
        g.in_next = g.in_which ? g.in_buf : g.in_buf2;
        g.in_left = g.in_len;

         
        if (g.in_len == BUF) {
            g.in_which = 1 - g.in_which;
            possess(g.load_state);
            twist(g.load_state, TO, 1);
        }

         
        else {
            join(g.load_thread);
            free_lock(g.load_state);
            g.in_which = -1;
        }
    }
    else
#endif
    {
         
        g.in_left = readn(g.ind, g.in_next = g.in_buf, BUF);
    }

     
    if (g.in_left < BUF) {
        g.in_short = 1;

         
        if (g.in_left == 0)
            g.in_eof = 1;
    }

     
    g.in_tot += g.in_left;
    return g.in_left;
}
