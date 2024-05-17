local int outb(void *desc, unsigned char *buf, unsigned len)
{
#ifndef NOTHREAD
    static thread *wr, *ch;

    if (g.procs > 1) {
         
        if (outb_write_more == NULL) {
            outb_write_more = new_lock(0);
            outb_check_more = new_lock(0);
            wr = launch(outb_write, NULL);
            ch = launch(outb_check, NULL);
        }

         
        possess(outb_check_more);
        wait_for(outb_check_more, TO_BE, 0);
        possess(outb_write_more);
        wait_for(outb_write_more, TO_BE, 0);

         
        out_len = len;
        g.out_tot += len;
        memcpy(out_copy, buf, len);
        twist(outb_write_more, TO, 1);
        twist(outb_check_more, TO, 1);

         
        if (len == 0) {
            join(ch);
            join(wr);
            free_lock(outb_check_more);
            free_lock(outb_write_more);
            outb_write_more = NULL;
        }

         
        return 0;
    }
#endif

    (void)desc;

     
    if (len) {
        if (g.decode == 1)
            writen(g.outd, buf, len);
        g.out_check = CHECK(g.out_check, buf, len);
        g.out_tot += len;
    }
    return 0;
}