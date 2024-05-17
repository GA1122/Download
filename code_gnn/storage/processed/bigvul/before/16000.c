static void destroy_tls_key(void *ptr)
{
    int *err_ptr = static_cast<int *>(ptr);
    delete err_ptr;
}
