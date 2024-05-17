void OPENSSL_thread_stop(void)
{
    if (destructor_key.sane != -1)
        ossl_init_thread_stop(ossl_init_get_thread_local(0));
}
