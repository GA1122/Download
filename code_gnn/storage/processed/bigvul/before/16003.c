static void set_error(int err)
{
    int *err_ptr;

    pthread_once(&key_once, create_tls_key);

    err_ptr = (int *)pthread_getspecific(key);

    if (err_ptr == NULL) {
        err_ptr = new int;
        pthread_setspecific(key, err_ptr);
    }

    *err_ptr = err;
}
