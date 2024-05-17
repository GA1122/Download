const SSL_CIPHER *ssl2_get_cipher(unsigned int u)
{
    if (u < SSL2_NUM_CIPHERS)
        return (&(ssl2_ciphers[SSL2_NUM_CIPHERS - 1 - u]));
    else
        return (NULL);
}
