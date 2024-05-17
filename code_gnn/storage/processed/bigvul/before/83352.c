static int ssl_parse_certificate_request( mbedtls_ssl_context *ssl )
{
    int ret;
    unsigned char *buf;
    size_t n = 0;
    size_t cert_type_len = 0, dn_len = 0;
    const mbedtls_ssl_ciphersuite_t *ciphersuite_info =
        ssl->transform_negotiate->ciphersuite_info;

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "=> parse certificate request" ) );

    if( ! mbedtls_ssl_ciphersuite_cert_req_allowed( ciphersuite_info ) )
    {
        MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= skip parse certificate request" ) );
        ssl->state++;
        return( 0 );
    }

    if( ( ret = mbedtls_ssl_read_record( ssl ) ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ssl_read_record", ret );
        return( ret );
    }

    if( ssl->in_msgtype != MBEDTLS_SSL_MSG_HANDSHAKE )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad certificate request message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_UNEXPECTED_MESSAGE );
        return( MBEDTLS_ERR_SSL_UNEXPECTED_MESSAGE );
    }

    ssl->state++;
    ssl->client_auth = ( ssl->in_msg[0] == MBEDTLS_SSL_HS_CERTIFICATE_REQUEST );

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "got %s certificate request",
                        ssl->client_auth ? "a" : "no" ) );

    if( ssl->client_auth == 0 )
    {
         
        ssl->keep_current_message = 1;
        goto exit;
    }

     
    buf = ssl->in_msg;

     
    cert_type_len = buf[mbedtls_ssl_hs_hdr_len( ssl )];
    n = cert_type_len;

    if( ssl->in_hslen < mbedtls_ssl_hs_hdr_len( ssl ) + 2 + n )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad certificate request message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
        return( MBEDTLS_ERR_SSL_BAD_HS_CERTIFICATE_REQUEST );
    }

     
#if defined(MBEDTLS_SSL_PROTO_TLS1_2)
    if( ssl->minor_ver == MBEDTLS_SSL_MINOR_VERSION_3 )
    {
        size_t sig_alg_len = ( ( buf[mbedtls_ssl_hs_hdr_len( ssl ) + 1 + n] <<  8 )
                             | ( buf[mbedtls_ssl_hs_hdr_len( ssl ) + 2 + n]       ) );
#if defined(MBEDTLS_DEBUG_C)
        unsigned char* sig_alg = buf + mbedtls_ssl_hs_hdr_len( ssl ) + 3 + n;
        size_t i;

        for( i = 0; i < sig_alg_len; i += 2 )
        {
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "Supported Signature Algorithm found: %d"
                                        ",%d", sig_alg[i], sig_alg[i + 1]  ) );
        }
#endif

        n += 2 + sig_alg_len;

        if( ssl->in_hslen < mbedtls_ssl_hs_hdr_len( ssl ) + 2 + n )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad certificate request message" ) );
            mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                            MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
            return( MBEDTLS_ERR_SSL_BAD_HS_CERTIFICATE_REQUEST );
        }
    }
#endif  

     
    dn_len = ( ( buf[mbedtls_ssl_hs_hdr_len( ssl ) + 1 + n] <<  8 )
             | ( buf[mbedtls_ssl_hs_hdr_len( ssl ) + 2 + n]       ) );

    n += dn_len;
    if( ssl->in_hslen != mbedtls_ssl_hs_hdr_len( ssl ) + 3 + n )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad certificate request message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
        return( MBEDTLS_ERR_SSL_BAD_HS_CERTIFICATE_REQUEST );
    }

exit:
    MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= parse certificate request" ) );

    return( 0 );
}
