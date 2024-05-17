static int dtls1_process_buffered_records(SSL *s)
{
    pitem *item;
    SSL3_BUFFER *rb;
    SSL3_RECORD *rr;
    DTLS1_BITMAP *bitmap;
    unsigned int is_next_epoch;
    int replayok = 1;

    item = pqueue_peek(s->d1->unprocessed_rcds.q);
    if (item) {
         
        if (s->d1->unprocessed_rcds.epoch != s->d1->r_epoch)
            return 1;          

        rr = &s->s3->rrec;
        rb = &s->s3->rbuf;

        if (rb->left > 0) {
             
            return 1;
        }


         
        while (pqueue_peek(s->d1->unprocessed_rcds.q)) {
            dtls1_get_unprocessed_record(s);
            bitmap = dtls1_get_bitmap(s, rr, &is_next_epoch);
            if (bitmap == NULL) {
                 
                 SSLerr(SSL_F_DTLS1_PROCESS_BUFFERED_RECORDS,
                        ERR_R_INTERNAL_ERROR);
                 return 0;
            }
#ifndef OPENSSL_NO_SCTP
             
            if (!BIO_dgram_is_sctp(SSL_get_rbio(s)))
#endif
            {
                 
                replayok = dtls1_record_replay_check(s, bitmap);
            }

            if (!replayok || !dtls1_process_record(s, bitmap)) {
                 
                rr->length = 0;
                s->packet_length = 0;
                continue;
            }

            if (dtls1_buffer_record(s, &(s->d1->processed_rcds),
                                    s->s3->rrec.seq_num) < 0)
                return 0;
        }
    }

     
    s->d1->processed_rcds.epoch = s->d1->r_epoch;
    s->d1->unprocessed_rcds.epoch = s->d1->r_epoch + 1;

    return 1;
}
