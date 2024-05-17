static void init_read_state_machine(SSL *s)
{
    OSSL_STATEM *st = &s->statem;

    st->read_state = READ_STATE_HEADER;
}
