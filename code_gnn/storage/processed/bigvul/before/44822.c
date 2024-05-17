local void parallel_compress(void)
{
    long seq;                        
    struct space *curr;              
    struct space *next;              
    struct space *hold;              
    struct space *dict;              
    struct job *job;                 
    int more;                        
    unsigned hash;                   
    unsigned char *scan;             
    unsigned char *end;              
    unsigned char *last;             
    size_t left;                     
    size_t len;                      

     
    setup_jobs();

     
    writeth = launch(write_thread, NULL);

     
    seq = 0;
    next = get_space(&in_pool);
    next->len = readn(g.ind, next->buf, next->size);
    hold = NULL;
    dict = NULL;
    scan = next->buf;
    hash = RSYNCHIT;
    left = 0;
    do {
         
        job = MALLOC(sizeof(struct job));
        if (job == NULL)
            bail("not enough memory", "");
        job->calc = new_lock(0);

         
        curr = next;
        next = hold;
        hold = NULL;

         
        if (next == NULL) {
            next = get_space(&in_pool);
            next->len = readn(g.ind, next->buf, next->size);
        }

         
        job->lens = NULL;
        if (g.rsync && curr->len) {
             
            if (left == 0) {
                 
                last = curr->buf;
                end = curr->buf + curr->len;
                while (scan < end) {
                    hash = ((hash << 1) ^ *scan++) & RSYNCMASK;
                    if (hash == RSYNCHIT) {
                        len = scan - last;
                        append_len(job, len);
                        last = scan;
                    }
                }

                 
                left = scan - last;
                scan = next->buf;
            }

             
            last = next->buf;
            len = curr->size - curr->len;
            if (len > next->len)
                len = next->len;
            end = next->buf + len;
            while (scan < end) {
                hash = ((hash << 1) ^ *scan++) & RSYNCMASK;
                if (hash == RSYNCHIT) {
                    len = (scan - last) + left;
                    left = 0;
                    append_len(job, len);
                    last = scan;
                }
            }
            append_len(job, 0);

             
            len = (job->lens->len == 1 ? scan : last) - next->buf;
            if (len) {
                 
                memcpy(curr->buf + curr->len, next->buf, len);
                curr->len += len;
                memmove(next->buf, next->buf + len, next->len - len);
                next->len -= len;
                scan -= len;
                left = 0;
            }
            else if (job->lens->len != 1 && left && next->len) {
                 
                hold = next;
                next = get_space(&in_pool);
                memcpy(next->buf, curr->buf + (curr->len - left), left);
                next->len = left;
                curr->len -= left;
            }
            else {
                 
                left = 0;
            }
        }

         
        job->in = curr;

         
        more = next->len != 0;
        job->more = more;

         
        job->out = dict;
        if (more && g.setdict) {
            if (curr->len >= DICT || job->out == NULL) {
                dict = curr;
                use_space(dict);
            }
            else {
                dict = get_space(&dict_pool);
                len = DICT - curr->len;
                memcpy(dict->buf, job->out->buf + (job->out->len - len), len);
                memcpy(dict->buf + len, curr->buf, curr->len);
                dict->len = DICT;
            }
        }

         
        job->seq = seq;
        Trace(("-- read #%ld%s", seq, more ? "" : " (last)"));
        if (++seq < 1)
            bail("input too long: ", g.inf);

         
        if (cthreads < seq && cthreads < g.procs) {
            (void)launch(compress_thread, NULL);
            cthreads++;
        }

         
        possess(compress_have);
        job->next = NULL;
        *compress_tail = job;
        compress_tail = &(job->next);
        twist(compress_have, BY, +1);
    } while (more);
    drop_space(next);

     
    join(writeth);
    writeth = NULL;
    Trace(("-- write thread joined"));
}
