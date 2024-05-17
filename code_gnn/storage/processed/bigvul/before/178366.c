 int dtls1_process_buffered_records(SSL *s)
  {
      pitem *item;
      SSL3_BUFFER *rb;
//     SSL3_RECORD *rr;
//     DTLS1_BITMAP *bitmap;
//     unsigned int is_next_epoch;
//     int replayok = 1;
  
      item = pqueue_peek(s->rlayer.d->unprocessed_rcds.q);
      if (item) {
           
          if (s->rlayer.d->unprocessed_rcds.epoch != s->rlayer.d->r_epoch)
            return (1);          
//             return 1;          
// 
//         rr = RECORD_LAYER_get_rrec(&s->rlayer);
  
          rb = RECORD_LAYER_get_rbuf(&s->rlayer);
  
              */
             return 1;
         }
 
          
         while (pqueue_peek(s->rlayer.d->unprocessed_rcds.q)) {
             dtls1_get_unprocessed_record(s);
             if (!dtls1_process_record(s))
                 return (0);
             if (dtls1_buffer_record(s, &(s->rlayer.d->processed_rcds),
           
          while (pqueue_peek(s->rlayer.d->unprocessed_rcds.q)) {
              dtls1_get_unprocessed_record(s);
            if (!dtls1_process_record(s))
                return (0);
//             bitmap = dtls1_get_bitmap(s, rr, &is_next_epoch);
//             if (bitmap == NULL) {
//                  
//                  SSLerr(SSL_F_DTLS1_PROCESS_BUFFERED_RECORDS,
//                         ERR_R_INTERNAL_ERROR);
//                  return 0;
//             }
// #ifndef OPENSSL_NO_SCTP
//              
//             if (!BIO_dgram_is_sctp(SSL_get_rbio(s)))
// #endif
//             {
//                  
//                 replayok = dtls1_record_replay_check(s, bitmap);
//             }
// 
//             if (!replayok || !dtls1_process_record(s, bitmap)) {
//                  
//                 rr->length = 0;
//                 RECORD_LAYER_reset_packet_length(&s->rlayer);
//                 continue;
//             }
// 
              if (dtls1_buffer_record(s, &(s->rlayer.d->processed_rcds),
                                    SSL3_RECORD_get_seq_num(s->rlayer.rrec)) <
                0)
                return -1;
//                     SSL3_RECORD_get_seq_num(s->rlayer.rrec)) < 0)
//                 return 0;
          }
      }
  
  *             here, anything else is handled by higher layers
  *     Application data protocol
  *             none of our business
  */
      s->rlayer.d->processed_rcds.epoch = s->rlayer.d->r_epoch;
      s->rlayer.d->unprocessed_rcds.epoch = s->rlayer.d->r_epoch + 1;
  
    return (1);
//     return 1;
  }