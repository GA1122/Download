static SUB_STATE_RETURN write_state_machine(SSL *s)
{
    OSSL_STATEM *st = &s->statem;
    int ret;
    WRITE_TRAN(*transition) (SSL *s);
    WORK_STATE(*pre_work) (SSL *s, WORK_STATE wst);
    WORK_STATE(*post_work) (SSL *s, WORK_STATE wst);
    int (*construct_message) (SSL *s);
    void (*cb) (const SSL *ssl, int type, int val) = NULL;

    cb = get_callback(s);

    if (s->server) {
        transition = ossl_statem_server_write_transition;
        pre_work = ossl_statem_server_pre_work;
        post_work = ossl_statem_server_post_work;
        construct_message = ossl_statem_server_construct_message;
    } else {
        transition = ossl_statem_client_write_transition;
        pre_work = ossl_statem_client_pre_work;
        post_work = ossl_statem_client_post_work;
        construct_message = ossl_statem_client_construct_message;
    }

    while (1) {
        switch (st->write_state) {
        case WRITE_STATE_TRANSITION:
            if (cb != NULL) {
                 
                if (s->server)
                    cb(s, SSL_CB_ACCEPT_LOOP, 1);
                else
                    cb(s, SSL_CB_CONNECT_LOOP, 1);
            }
            switch (transition(s)) {
            case WRITE_TRAN_CONTINUE:
                st->write_state = WRITE_STATE_PRE_WORK;
                st->write_state_work = WORK_MORE_A;
                break;

            case WRITE_TRAN_FINISHED:
                return SUB_STATE_FINISHED;
                break;

            default:
                return SUB_STATE_ERROR;
            }
            break;

        case WRITE_STATE_PRE_WORK:
            switch (st->write_state_work = pre_work(s, st->write_state_work)) {
            default:
                return SUB_STATE_ERROR;

            case WORK_FINISHED_CONTINUE:
                st->write_state = WRITE_STATE_SEND;
                break;

            case WORK_FINISHED_STOP:
                return SUB_STATE_END_HANDSHAKE;
            }
            if (construct_message(s) == 0)
                return SUB_STATE_ERROR;

             

        case WRITE_STATE_SEND:
            if (SSL_IS_DTLS(s) && st->use_timer) {
                dtls1_start_timer(s);
            }
            ret = statem_do_write(s);
            if (ret <= 0) {
                return SUB_STATE_ERROR;
            }
            st->write_state = WRITE_STATE_POST_WORK;
            st->write_state_work = WORK_MORE_A;
             

        case WRITE_STATE_POST_WORK:
            switch (st->write_state_work = post_work(s, st->write_state_work)) {
            default:
                return SUB_STATE_ERROR;

            case WORK_FINISHED_CONTINUE:
                st->write_state = WRITE_STATE_TRANSITION;
                break;

            case WORK_FINISHED_STOP:
                return SUB_STATE_END_HANDSHAKE;
            }
            break;

        default:
            return SUB_STATE_ERROR;
        }
    }
}
