local void write_thread(void *dummy)
{
    long seq;                        
    struct job *job;                 
    size_t len;                      
    int more;                        
    unsigned long head;              
    unsigned long ulen;              
    unsigned long clen;              
    unsigned long check;             

    (void)dummy;

     
    Trace(("-- write thread running"));
    head = put_header();

     
    ulen = clen = 0;
    check = CHECK(0L, Z_NULL, 0);
    seq = 0;
    do {
         
        possess(write_first);
        wait_for(write_first, TO_BE, seq);
        job = write_head;
        write_head = job->next;
        twist(write_first, TO, write_head == NULL ? -1 : write_head->seq);

         
        more = job->more;
        len = job->in->len;
        drop_space(job->in);
        ulen += (unsigned long)len;
        clen += (unsigned long)(job->out->len);

         
        Trace(("-- writing #%ld", seq));
        writen(g.outd, job->out->buf, job->out->len);
        drop_space(job->out);
        Trace(("-- wrote #%ld%s", seq, more ? "" : " (last)"));

         
        possess(job->calc);
        wait_for(job->calc, TO_BE, 1);
        release(job->calc);
        check = COMB(check, job->check, len);

         
        free_lock(job->calc);
        FREE(job);

         
        seq++;
    } while (more);

     
    put_trailer(ulen, clen, check, head);

     
    possess(compress_have);
    assert(compress_head == NULL && peek_lock(compress_have) == 0);
    release(compress_have);
    possess(write_first);
    assert(write_head == NULL);
    twist(write_first, TO, -1);
}
