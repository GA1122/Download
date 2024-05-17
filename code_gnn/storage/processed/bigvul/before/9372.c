int ossl_statem_app_data_allowed(SSL *s)
{
    OSSL_STATEM *st = &s->statem;

    if (st->state == MSG_FLOW_UNINITED || st->state == MSG_FLOW_RENEGOTIATE)
        return 0;

    if (!s->s3->in_read_app_data || (s->s3->total_renegotiations == 0))
        return 0;

    if (s->server) {
         
        if (st->hand_state == TLS_ST_BEFORE
            || st->hand_state == TLS_ST_SR_CLNT_HELLO)
            return 1;
    } else {
         
        if (st->hand_state == TLS_ST_CW_CLNT_HELLO)
            return 1;
    }

    return 0;
}
