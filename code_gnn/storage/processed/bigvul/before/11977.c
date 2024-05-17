static DTLS1_BITMAP *dtls1_get_bitmap(SSL *s, SSL3_RECORD *rr,
                                      unsigned int *is_next_epoch)
{

    *is_next_epoch = 0;

     
    if (rr->epoch == s->d1->r_epoch)
        return &s->d1->bitmap;

     
    else if (rr->epoch == (unsigned long)(s->d1->r_epoch + 1) &&
             s->d1->unprocessed_rcds.epoch != s->d1->r_epoch &&
             (rr->type == SSL3_RT_HANDSHAKE || rr->type == SSL3_RT_ALERT)) {
        *is_next_epoch = 1;
        return &s->d1->next_bitmap;
    }

    return NULL;
}
