local void compress_thread(void *dummy)
{
    struct job *job;                 
    struct job *here, **prior;       
    unsigned long check;             
    unsigned char *next;             
    size_t left;                     
    size_t len;                      
#if ZLIB_VERNUM >= 0x1260
    int bits;                        
#endif
    struct space *temp = NULL;       
    z_stream strm;                   

    (void)dummy;

     
    strm.zfree = ZFREE;
    strm.zalloc = ZALLOC;
    strm.opaque = OPAQUE;
    if (deflateInit2(&strm, 6, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY) != Z_OK)
        bail("not enough memory", "");

     
    for (;;) {
         
        possess(compress_have);
        wait_for(compress_have, NOT_TO_BE, 0);
        job = compress_head;
        assert(job != NULL);
        if (job->seq == -1)
            break;
        compress_head = job->next;
        if (job->next == NULL)
            compress_tail = &compress_head;
        twist(compress_have, BY, -1);

         
        Trace(("-- compressing #%ld", job->seq));
        if (g.level <= 9) {
            (void)deflateReset(&strm);
            (void)deflateParams(&strm, g.level, Z_DEFAULT_STRATEGY);
        }
        else {
            temp = get_space(&out_pool);
            temp->len = 0;
        }

         
        if (job->out != NULL) {
            len = job->out->len;
            left = len < DICT ? len : DICT;
            if (g.level <= 9)
                deflateSetDictionary(&strm, job->out->buf + (len - left),
                                     left);
            else {
                memcpy(temp->buf, job->out->buf + (len - left), left);
                temp->len = left;
            }
            drop_space(job->out);
        }

         
        job->out = get_space(&out_pool);
        if (g.level <= 9) {
            strm.next_in = job->in->buf;
            strm.next_out = job->out->buf;
        }
        else
            memcpy(temp->buf + temp->len, job->in->buf, job->in->len);

         
        next = job->lens == NULL ? NULL : job->lens->buf;
        left = job->in->len;
        job->out->len = 0;
        do {
             
            len = next == NULL ? 128 : *next++;
            if (len < 128)                           
                len = (len << 8) + (*next++) + 64;
            else if (len == 128)                     
                len = left;
            else if (len < 192)                      
                len &= 0x3f;
            else if (len < 224){                     
                len = ((len & 0x1f) << 16) + (*next++ << 8);
                len += *next++ + 32832U;
            }
            else {                                   
                len = ((len & 0x1f) << 24) + (*next++ << 16);
                len += *next++ << 8;
                len += *next++ + 2129984UL;
            }
            left -= len;

            if (g.level <= 9) {
                 
                while (len > MAXP2) {
                    strm.avail_in = MAXP2;
                    deflate_engine(&strm, job->out, Z_NO_FLUSH);
                    len -= MAXP2;
                }

                 
                strm.avail_in = (unsigned)len;
                if (left || job->more) {
#if ZLIB_VERNUM >= 0x1260
                    deflate_engine(&strm, job->out, Z_BLOCK);

                     
                    (void)deflatePending(&strm, Z_NULL, &bits);
                    if (bits & 1)
                        deflate_engine(&strm, job->out, Z_SYNC_FLUSH);
                    else if (bits & 7) {
                        do {         
                            bits = deflatePrime(&strm, 10, 2);
                            assert(bits == Z_OK);
                            (void)deflatePending(&strm, Z_NULL, &bits);
                        } while (bits & 7);
                        deflate_engine(&strm, job->out, Z_BLOCK);
                    }
#else
                    deflate_engine(&strm, job->out, Z_SYNC_FLUSH);
#endif
                }
                else
                    deflate_engine(&strm, job->out, Z_FINISH);
            }
            else {
                 
                unsigned char bits, *out;
                size_t outsize;

                out = NULL;
                outsize = 0;
                bits = 0;
                ZopfliDeflatePart(&g.zopts, 2, !(left || job->more),
                                  temp->buf, temp->len, temp->len + len,
                                  &bits, &out, &outsize);
                assert(job->out->len + outsize + 5 <= job->out->size);
                memcpy(job->out->buf + job->out->len, out, outsize);
                free(out);
                job->out->len += outsize;
                if (left || job->more) {
                    bits &= 7;
                    if (bits & 1) {
                        if (bits == 7)
                            job->out->buf[job->out->len++] = 0;
                        job->out->buf[job->out->len++] = 0;
                        job->out->buf[job->out->len++] = 0;
                        job->out->buf[job->out->len++] = 0xff;
                        job->out->buf[job->out->len++] = 0xff;
                    }
                    else if (bits) {
                        do {
                            job->out->buf[job->out->len - 1] += 2 << bits;
                            job->out->buf[job->out->len++] = 0;
                            bits += 2;
                        } while (bits < 8);
                    }
                }
                temp->len += len;
            }
        } while (left);
        if (g.level > 9)
            drop_space(temp);
        if (job->lens != NULL) {
            drop_space(job->lens);
            job->lens = NULL;
        }
        Trace(("-- compressed #%ld%s", job->seq, job->more ? "" : " (last)"));

         
        use_space(job->in);

         
        possess(write_first);
        prior = &write_head;
        while ((here = *prior) != NULL) {
            if (here->seq > job->seq)
                break;
            prior = &(here->next);
        }
        job->next = here;
        *prior = job;
        twist(write_first, TO, write_head->seq);

         
        len = job->in->len;
        next = job->in->buf;
        check = CHECK(0L, Z_NULL, 0);
        while (len > MAXP2) {
            check = CHECK(check, next, MAXP2);
            len -= MAXP2;
            next += MAXP2;
        }
        check = CHECK(check, next, (unsigned)len);
        drop_space(job->in);
        job->check = check;
        Trace(("-- checked #%ld%s", job->seq, job->more ? "" : " (last)"));
        possess(job->calc);
        twist(job->calc, TO, 1);

         
    }

     
    release(compress_have);
    (void)deflateEnd(&strm);
}
