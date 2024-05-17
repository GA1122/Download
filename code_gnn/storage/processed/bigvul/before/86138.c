static int ssl_pick_cert( mbedtls_ssl_context *ssl,
                          const mbedtls_ssl_ciphersuite_t * ciphersuite_info )
{
    mbedtls_ssl_key_cert *cur, *list, *fallback = NULL;
    mbedtls_pk_type_t pk_alg =
        mbedtls_ssl_get_ciphersuite_sig_pk_alg( ciphersuite_info );
    uint32_t flags;

#if defined(MBEDTLS_SSL_SERVER_NAME_INDICATION)
    if( ssl->handshake->sni_key_cert != NULL )
        list = ssl->handshake->sni_key_cert;
    else
#endif
        list = ssl->conf->key_cert;

    if( pk_alg == MBEDTLS_PK_NONE )
        return( 0 );

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "ciphersuite requires certificate" ) );

    if( list == NULL )
    {
        MBEDTLS_SSL_DEBUG_MSG( 3, ( "server has no certificate" ) );
        return( -1 );
    }

    for( cur = list; cur != NULL; cur = cur->next )
    {
        MBEDTLS_SSL_DEBUG_CRT( 3, "candidate certificate chain, certificate",
                          cur->cert );

        if( ! mbedtls_pk_can_do( cur->key, pk_alg ) )
        {
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "certificate mismatch: key type" ) );
            continue;
        }

         
        if( mbedtls_ssl_check_cert_usage( cur->cert, ciphersuite_info,
                                  MBEDTLS_SSL_IS_SERVER, &flags ) != 0 )
        {
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "certificate mismatch: "
                                "(extended) key usage extension" ) );
            continue;
        }

#if defined(MBEDTLS_ECDSA_C)
        if( pk_alg == MBEDTLS_PK_ECDSA &&
            ssl_check_key_curve( cur->key, ssl->handshake->curves ) != 0 )
        {
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "certificate mismatch: elliptic curve" ) );
            continue;
        }
#endif

         
        if( ssl->minor_ver < MBEDTLS_SSL_MINOR_VERSION_3 &&
            cur->cert->sig_md != MBEDTLS_MD_SHA1 )
        {
            if( fallback == NULL )
                fallback = cur;
            {
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "certificate not preferred: "
                                    "sha-2 with pre-TLS 1.2 client" ) );
            continue;
            }
        }

         
        break;
    }

    if( cur == NULL )
        cur = fallback;

     
    if( cur != NULL )
    {
        ssl->handshake->key_cert = cur;
        MBEDTLS_SSL_DEBUG_CRT( 3, "selected certificate chain, certificate",
                          ssl->handshake->key_cert->cert );
        return( 0 );
    }

    return( -1 );
}