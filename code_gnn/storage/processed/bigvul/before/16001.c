static int get_error_for_thread()
{
    int *err_ptr;

    pthread_once(&key_once, create_tls_key);
    err_ptr = (int *)pthread_getspecific(key);

    if (err_ptr == NULL) {
        return 0;
    }

    return *err_ptr;
}
