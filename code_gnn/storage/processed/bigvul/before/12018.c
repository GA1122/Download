static void ossl_init_thread_destructor(void *local)
{
    ossl_init_thread_stop((struct thread_local_inits_st *)local);
}
