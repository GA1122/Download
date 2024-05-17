static void ssl_write_encrypt_then_mac_ext( mbedtls_ssl_context *ssl,
                                            unsigned char *buf,
                                            size_t *olen )
{
    unsigned char *p = buf;
    const mbedtls_ssl_ciphersuite_t *suite = NULL;
    const mbedtls_cipher_info_t *cipher = NULL;

    if( ssl->session_negotiate->encrypt_then_mac == MBEDTLS_SSL_EXTENDED_MS_DISABLED ||
        ssl->minor_ver == MBEDTLS_SSL_MINOR_VERSION_0 )
    {
        *olen = 0;
        return;
    }

     
    if( ( suite = mbedtls_ssl_ciphersuite_from_id(
                    ssl->session_negotiate->ciphersuite ) ) == NULL ||
        ( cipher = mbedtls_cipher_info_from_type( suite->cipher ) ) == NULL ||
        cipher->mode != MBEDTLS_MODE_CBC )
    {
        *olen = 0;
        return;
    }

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, adding encrypt then mac extension" ) );

    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_ENCRYPT_THEN_MAC >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_ENCRYPT_THEN_MAC      ) & 0xFF );

    *p++ = 0x00;
    *p++ = 0x00;

    *olen = 4;
}
