void RECORD_LAYER_clear(RECORD_LAYER *rl)
{
    unsigned int pipes;

    rl->rstate = SSL_ST_READ_HEADER;

     

    rl->packet = NULL;
    rl->packet_length = 0;
    rl->wnum = 0;
    memset(rl->alert_fragment, 0, sizeof(rl->alert_fragment));
    rl->alert_fragment_len = 0;
    memset(rl->handshake_fragment, 0, sizeof(rl->handshake_fragment));
    rl->handshake_fragment_len = 0;
    rl->wpend_tot = 0;
    rl->wpend_type = 0;
    rl->wpend_ret = 0;
    rl->wpend_buf = NULL;

    SSL3_BUFFER_clear(&rl->rbuf);
    for (pipes = 0; pipes < rl->numwpipes; pipes++)
        SSL3_BUFFER_clear(&rl->wbuf[pipes]);
    rl->numwpipes = 0;
    rl->numrpipes = 0;
    SSL3_RECORD_clear(rl->rrec, SSL_MAX_PIPELINES);

    RECORD_LAYER_reset_read_sequence(rl);
    RECORD_LAYER_reset_write_sequence(rl);

    if (rl->d)
        DTLS_RECORD_LAYER_clear(rl);
}
