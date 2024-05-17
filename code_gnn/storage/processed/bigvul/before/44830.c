local void single_compress(int reset)
{
    size_t got;                      
    size_t more;                     
    size_t start;                    
    size_t have;                     
    size_t hist;                     
    int fresh;                       
    unsigned hash;                   
    unsigned char *scan;             
    size_t left;                     
    unsigned long head;              
    unsigned long ulen;              
    unsigned long clen;              
    unsigned long check;             
    static unsigned out_size;        
    static unsigned char *in, *next, *out;   
    static z_stream *strm = NULL;    

     
    if (reset) {
        if (strm != NULL) {
            (void)deflateEnd(strm);
            FREE(strm);
            FREE(out);
            FREE(next);
            FREE(in);
            strm = NULL;
        }
        return;
    }

     
    if (strm == NULL) {
        out_size = g.block > MAXP2 ? MAXP2 : (unsigned)g.block;
        if ((in = MALLOC(g.block + DICT)) == NULL ||
            (next = MALLOC(g.block + DICT)) == NULL ||
            (out = MALLOC(out_size)) == NULL ||
            (strm = MALLOC(sizeof(z_stream))) == NULL)
            bail("not enough memory", "");
        strm->zfree = ZFREE;
        strm->zalloc = ZALLOC;
        strm->opaque = OPAQUE;
        if (deflateInit2(strm, 6, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY) !=
                         Z_OK)
            bail("not enough memory", "");
    }

     
    head = put_header();

     
    if (g.level <= 9) {
        (void)deflateReset(strm);
        (void)deflateParams(strm, g.level, Z_DEFAULT_STRATEGY);
    }

     
    got = 0;
    more = readn(g.ind, next, g.block);
    ulen = (unsigned long)more;
    start = 0;
    hist = 0;
    clen = 0;
    have = 0;
    check = CHECK(0L, Z_NULL, 0);
    hash = RSYNCHIT;
    do {
         
        if (got == 0) {
            scan = in;  in = next;  next = scan;
            strm->next_in = in + start;
            got = more;
            if (g.level > 9) {
                left = start + more - hist;
                if (left > DICT)
                    left = DICT;
                memcpy(next, in + ((start + more) - left), left);
                start = left;
                hist = 0;
            }
            else
                start = 0;
            more = readn(g.ind, next + start, g.block);
            ulen += (unsigned long)more;
        }

         
        left = 0;
        if (g.rsync && got) {
            scan = strm->next_in;
            left = got;
            do {
                if (left == 0) {
                     
                    if (more == 0 || got == g.block)
                        break;

                     
                    if (g.level > 9) {
                        left = (strm->next_in - in) - hist;
                        if (left > DICT)
                            left = DICT;
                    }
                    memmove(in, strm->next_in - left, left + got);
                    hist = 0;
                    strm->next_in = in + left;
                    scan = in + left + got;
                    left = more > g.block - got ? g.block - got : more;
                    memcpy(scan, next + start, left);
                    got += left;
                    more -= left;
                    start += left;

                     
                    if (more == 0) {
                        more = readn(g.ind, next, g.block);
                        ulen += (unsigned long)more;
                        start = 0;
                    }
                }
                left--;
                hash = ((hash << 1) ^ *scan++) & RSYNCMASK;
            } while (hash != RSYNCHIT);
            got -= left;
        }

         
        fresh = 0;
        if (!g.setdict) {
            have += got;
            if (have > g.block) {
                fresh = 1;
                have = got;
            }
        }

        if (g.level <= 9) {
             
            if (fresh)
                (void)deflateReset(strm);

             
            while (got > MAXP2) {
                strm->avail_in = MAXP2;
                check = CHECK(check, strm->next_in, strm->avail_in);
                DEFLATE_WRITE(Z_NO_FLUSH);
                got -= MAXP2;
            }

             
            strm->avail_in = (unsigned)got;
            got = left;
            check = CHECK(check, strm->next_in, strm->avail_in);
            if (more || got) {
#if ZLIB_VERNUM >= 0x1260
                int bits;

                DEFLATE_WRITE(Z_BLOCK);
                (void)deflatePending(strm, Z_NULL, &bits);
                if (bits & 1)
                    DEFLATE_WRITE(Z_SYNC_FLUSH);
                else if (bits & 7) {
                    do {
                        bits = deflatePrime(strm, 10, 2);
                        assert(bits == Z_OK);
                        (void)deflatePending(strm, Z_NULL, &bits);
                    } while (bits & 7);
                    DEFLATE_WRITE(Z_NO_FLUSH);
                }
#else
                DEFLATE_WRITE(Z_SYNC_FLUSH);
#endif
            }
            else
                DEFLATE_WRITE(Z_FINISH);
        }
        else {
             
            unsigned char bits, *out;
            size_t outsize, off;

             
            off = strm->next_in - in;
            if (fresh)
                hist = off;

            out = NULL;
            outsize = 0;
            bits = 0;
            ZopfliDeflatePart(&g.zopts, 2, !(more || left),
                              in + hist, off - hist, (off - hist) + got,
                              &bits, &out, &outsize);
            bits &= 7;
            if ((more || left) && bits) {
                if (bits & 1) {
                    writen(g.outd, out, outsize);
                    if (bits == 7)
                        writen(g.outd, (unsigned char *)"\0", 1);
                    writen(g.outd, (unsigned char *)"\0\0\xff\xff", 4);
                }
                else {
                    assert(outsize > 0);
                    writen(g.outd, out, outsize - 1);
                    do {
                        out[outsize - 1] += 2 << bits;
                        writen(g.outd, out + outsize - 1, 1);
                        out[outsize - 1] = 0;
                        bits += 2;
                    } while (bits < 8);
                    writen(g.outd, out + outsize - 1, 1);
                }
            }
            else
                writen(g.outd, out, outsize);
            free(out);
            while (got > MAXP2) {
                check = CHECK(check, strm->next_in, MAXP2);
                strm->next_in += MAXP2;
                got -= MAXP2;
            }
            check = CHECK(check, strm->next_in, (unsigned)got);
            strm->next_in += got;
            got = left;
        }

         
    } while (more || got);

     
    put_trailer(ulen, clen, check, head);
}
