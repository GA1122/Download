static void init_write_state_machine(SSL *s)
{
    OSSL_STATEM *st = &s->statem;

    st->write_state = WRITE_STATE_TRANSITION;
}
