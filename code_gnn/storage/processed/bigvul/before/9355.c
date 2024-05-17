int ossl_statem_connect(SSL *s)
{
    return state_machine(s, 0);
}
