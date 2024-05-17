static void create_tls_key()
{
    (void)pthread_key_create(&key, destroy_tls_key);
}
