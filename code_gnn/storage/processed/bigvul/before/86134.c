static int ssl_parse_signature_algorithms_ext( mbedtls_ssl_context *ssl,
                                               const unsigned char *buf,
                                               size_t len )
{
    size_t sig_alg_list_size;

    const unsigned char *p;
    const unsigned char *end = buf + len;

    mbedtls_md_type_t md_cur;
    mbedtls_pk_type_t sig_cur;

    sig_alg_list_size = ( ( buf[0] << 8 ) | ( buf[1] ) );
    if( sig_alg_list_size + 2 != len ||
        sig_alg_list_size % 2 != 0 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
        return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
    }

     

    for( p = buf + 2; p < end; p += 2 )
    {
         

        if( ( sig_cur = mbedtls_ssl_pk_alg_from_sig( p[1] ) ) == MBEDTLS_PK_NONE )
        {
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello v3, signature_algorithm ext"
                                        " unknown sig alg encoding %d", p[1] ) );
            continue;
        }

         
        md_cur = mbedtls_ssl_md_alg_from_hash( p[0] );
        if( md_cur == MBEDTLS_MD_NONE )
        {
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello v3, signature_algorithm ext:"
                                        " unknown hash alg encoding %d", p[0] ) );
            continue;
        }

        if( mbedtls_ssl_check_sig_hash( ssl, md_cur ) == 0 )
        {
            mbedtls_ssl_sig_hash_set_add( &ssl->handshake->hash_algs, sig_cur, md_cur );
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello v3, signature_algorithm ext:"
                                        " match sig %d and hash %d",
                                        sig_cur, md_cur ) );
        }
        else
        {
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello v3, signature_algorithm ext: "
                                        "hash alg %d not supported", md_cur ) );
        }
    }

    return( 0 );
}
