static int ssl_write_server_key_exchange( mbedtls_ssl_context *ssl )
{
    int ret;
    size_t n = 0;
    const mbedtls_ssl_ciphersuite_t *ciphersuite_info =
                            ssl->transform_negotiate->ciphersuite_info;

#if defined(MBEDTLS_KEY_EXCHANGE__SOME_PFS__ENABLED)
    unsigned char *p = ssl->out_msg + 4;
    size_t len;
#if defined(MBEDTLS_KEY_EXCHANGE__WITH_SERVER_SIGNATURE__ENABLED)
    unsigned char *dig_signed = p;
    size_t dig_signed_len = 0;
#endif  
#endif  

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "=> write server key exchange" ) );

     

     
#if defined(MBEDTLS_KEY_EXCHANGE__SOME__ECDH_ENABLED)
    if( mbedtls_ssl_ciphersuite_uses_ecdh( ciphersuite_info ) )
    {
        ssl_get_ecdh_params_from_cert( ssl );
    }
#endif  

     
#if defined(MBEDTLS_KEY_EXCHANGE__SOME_NON_PFS__ENABLED)
    if( mbedtls_ssl_ciphersuite_no_pfs( ciphersuite_info ) )
    {
        MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= skip write server key exchange" ) );
        ssl->state++;
        return( 0 );
    }
#endif  

     

     
#if defined(MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED)
    if( ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_ECJPAKE )
    {
        const unsigned char *end = ssl->out_msg + MBEDTLS_SSL_MAX_CONTENT_LEN;

        ret = mbedtls_ecjpake_write_round_two( &ssl->handshake->ecjpake_ctx,
                p, end - p, &len, ssl->conf->f_rng, ssl->conf->p_rng );
        if( ret != 0 )
        {
            MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ecjpake_write_round_two", ret );
            return( ret );
        }

        p += len;
        n += len;
    }
#endif  

     
#if defined(MBEDTLS_KEY_EXCHANGE_DHE_PSK_ENABLED)   || \
    defined(MBEDTLS_KEY_EXCHANGE_ECDHE_PSK_ENABLED)
    if( ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_DHE_PSK ||
        ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_ECDHE_PSK )
    {
        *(p++) = 0x00;
        *(p++) = 0x00;

        n += 2;
    }
#endif  

     
#if defined(MBEDTLS_KEY_EXCHANGE__SOME__DHE_ENABLED)
    if( mbedtls_ssl_ciphersuite_uses_dhe( ciphersuite_info ) )
    {
        if( ssl->conf->dhm_P.p == NULL || ssl->conf->dhm_G.p == NULL )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "no DH parameters set" ) );
            return( MBEDTLS_ERR_SSL_BAD_INPUT_DATA );
        }

         
        if( ( ret = mbedtls_mpi_copy( &ssl->handshake->dhm_ctx.P, &ssl->conf->dhm_P ) ) != 0 ||
            ( ret = mbedtls_mpi_copy( &ssl->handshake->dhm_ctx.G, &ssl->conf->dhm_G ) ) != 0 )
        {
            MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_mpi_copy", ret );
            return( ret );
        }

        if( ( ret = mbedtls_dhm_make_params( &ssl->handshake->dhm_ctx,
                        (int) mbedtls_mpi_size( &ssl->handshake->dhm_ctx.P ),
                        p, &len, ssl->conf->f_rng, ssl->conf->p_rng ) ) != 0 )
        {
            MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_dhm_make_params", ret );
            return( ret );
        }

#if defined(MBEDTLS_KEY_EXCHANGE__WITH_SERVER_SIGNATURE__ENABLED)        
        dig_signed = p;
        dig_signed_len = len;
#endif

        p += len;
        n += len;

        MBEDTLS_SSL_DEBUG_MPI( 3, "DHM: X ", &ssl->handshake->dhm_ctx.X  );
        MBEDTLS_SSL_DEBUG_MPI( 3, "DHM: P ", &ssl->handshake->dhm_ctx.P  );
        MBEDTLS_SSL_DEBUG_MPI( 3, "DHM: G ", &ssl->handshake->dhm_ctx.G  );
        MBEDTLS_SSL_DEBUG_MPI( 3, "DHM: GX", &ssl->handshake->dhm_ctx.GX );
    }
#endif  

     
#if defined(MBEDTLS_KEY_EXCHANGE__SOME__ECDHE_ENABLED)
    if( mbedtls_ssl_ciphersuite_uses_ecdhe( ciphersuite_info ) )
    {
         
        const mbedtls_ecp_curve_info **curve = NULL;
        const mbedtls_ecp_group_id *gid;

         
        for( gid = ssl->conf->curve_list; *gid != MBEDTLS_ECP_DP_NONE; gid++ )
            for( curve = ssl->handshake->curves; *curve != NULL; curve++ )
                if( (*curve)->grp_id == *gid )
                    goto curve_matching_done;

curve_matching_done:
        if( curve == NULL || *curve == NULL )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "no matching curve for ECDHE" ) );
            return( MBEDTLS_ERR_SSL_NO_CIPHER_CHOSEN );
        }

        MBEDTLS_SSL_DEBUG_MSG( 2, ( "ECDHE curve: %s", (*curve)->name ) );

        if( ( ret = mbedtls_ecp_group_load( &ssl->handshake->ecdh_ctx.grp,
                                       (*curve)->grp_id ) ) != 0 )
        {
            MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ecp_group_load", ret );
            return( ret );
        }

        if( ( ret = mbedtls_ecdh_make_params( &ssl->handshake->ecdh_ctx, &len,
                                      p, MBEDTLS_SSL_MAX_CONTENT_LEN - n,
                                      ssl->conf->f_rng, ssl->conf->p_rng ) ) != 0 )
        {
            MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ecdh_make_params", ret );
            return( ret );
        }

#if defined(MBEDTLS_KEY_EXCHANGE__WITH_SERVER_SIGNATURE__ENABLED)
        dig_signed     = p;
        dig_signed_len = len;
#endif

        p += len;
        n += len;

        MBEDTLS_SSL_DEBUG_ECP( 3, "ECDH: Q ", &ssl->handshake->ecdh_ctx.Q );
    }
#endif  

     
#if defined(MBEDTLS_KEY_EXCHANGE__WITH_SERVER_SIGNATURE__ENABLED)
    if( mbedtls_ssl_ciphersuite_uses_server_signature( ciphersuite_info ) )
    {
        size_t signature_len = 0;
        unsigned int hashlen = 0;
        unsigned char hash[64];

         

        mbedtls_md_type_t md_alg;

#if defined(MBEDTLS_SSL_PROTO_TLS1_2)
        mbedtls_pk_type_t sig_alg =
            mbedtls_ssl_get_ciphersuite_sig_pk_alg( ciphersuite_info );
        if( ssl->minor_ver == MBEDTLS_SSL_MINOR_VERSION_3 )
        {
             
            if( sig_alg == MBEDTLS_PK_NONE ||
                ( md_alg = mbedtls_ssl_sig_hash_set_find( &ssl->handshake->hash_algs,
                                                          sig_alg ) ) == MBEDTLS_MD_NONE )
            {
                MBEDTLS_SSL_DEBUG_MSG( 1, ( "should never happen" ) );
                 
                return( MBEDTLS_ERR_SSL_INTERNAL_ERROR );
            }
        }
        else
#endif  
#if defined(MBEDTLS_SSL_PROTO_SSL3) || defined(MBEDTLS_SSL_PROTO_TLS1) || \
    defined(MBEDTLS_SSL_PROTO_TLS1_1)
        if( ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA )
        {
             
            md_alg = MBEDTLS_MD_SHA1;
        }
        else
#endif  
        {
             
            md_alg = MBEDTLS_MD_NONE;
        }

        MBEDTLS_SSL_DEBUG_MSG( 3, ( "pick hash algorithm %d for signing", md_alg ) );

         
#if defined(MBEDTLS_SSL_PROTO_SSL3) || defined(MBEDTLS_SSL_PROTO_TLS1) || \
    defined(MBEDTLS_SSL_PROTO_TLS1_1)
        if( md_alg == MBEDTLS_MD_NONE )
        {
            mbedtls_md5_context mbedtls_md5;
            mbedtls_sha1_context mbedtls_sha1;

            mbedtls_md5_init(  &mbedtls_md5  );
            mbedtls_sha1_init( &mbedtls_sha1 );

             

            mbedtls_md5_starts( &mbedtls_md5 );
            mbedtls_md5_update( &mbedtls_md5, ssl->handshake->randbytes,  64 );
            mbedtls_md5_update( &mbedtls_md5, dig_signed, dig_signed_len );
            mbedtls_md5_finish( &mbedtls_md5, hash );

            mbedtls_sha1_starts( &mbedtls_sha1 );
            mbedtls_sha1_update( &mbedtls_sha1, ssl->handshake->randbytes,  64 );
            mbedtls_sha1_update( &mbedtls_sha1, dig_signed, dig_signed_len );
            mbedtls_sha1_finish( &mbedtls_sha1, hash + 16 );

            hashlen = 36;

            mbedtls_md5_free(  &mbedtls_md5  );
            mbedtls_sha1_free( &mbedtls_sha1 );
        }
        else
#endif  
#if defined(MBEDTLS_SSL_PROTO_TLS1) || defined(MBEDTLS_SSL_PROTO_TLS1_1) || \
    defined(MBEDTLS_SSL_PROTO_TLS1_2)
        if( md_alg != MBEDTLS_MD_NONE )
        {
            mbedtls_md_context_t ctx;
            const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type( md_alg );

            mbedtls_md_init( &ctx );

             
            hashlen = 0;

             
            if( ( ret = mbedtls_md_setup( &ctx, md_info, 0 ) ) != 0 )
            {
                MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_md_setup", ret );
                return( ret );
            }

            mbedtls_md_starts( &ctx );
            mbedtls_md_update( &ctx, ssl->handshake->randbytes, 64 );
            mbedtls_md_update( &ctx, dig_signed, dig_signed_len );
            mbedtls_md_finish( &ctx, hash );
            mbedtls_md_free( &ctx );
        }
        else
#endif  
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "should never happen" ) );
            return( MBEDTLS_ERR_SSL_INTERNAL_ERROR );
        }

        MBEDTLS_SSL_DEBUG_BUF( 3, "parameters hash", hash, hashlen != 0 ? hashlen :
            (unsigned int) ( mbedtls_md_get_size( mbedtls_md_info_from_type( md_alg ) ) ) );

         
        if( mbedtls_ssl_own_key( ssl ) == NULL )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "got no private key" ) );
            return( MBEDTLS_ERR_SSL_PRIVATE_KEY_REQUIRED );
        }

#if defined(MBEDTLS_SSL_PROTO_TLS1_2)
        if( ssl->minor_ver == MBEDTLS_SSL_MINOR_VERSION_3 )
        {
             

            *(p++) = mbedtls_ssl_hash_from_md_alg( md_alg );
            *(p++) = mbedtls_ssl_sig_from_pk_alg( sig_alg );

            n += 2;
        }
#endif  

        if( ( ret = mbedtls_pk_sign( mbedtls_ssl_own_key( ssl ), md_alg, hash, hashlen,
                        p + 2 , &signature_len, ssl->conf->f_rng, ssl->conf->p_rng ) ) != 0 )
        {
            MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_pk_sign", ret );
            return( ret );
        }

        *(p++) = (unsigned char)( signature_len >> 8 );
        *(p++) = (unsigned char)( signature_len      );
        n += 2;

        MBEDTLS_SSL_DEBUG_BUF( 3, "my signature", p, signature_len );

        n += signature_len;
    }
#endif  

     

    ssl->out_msglen  = 4 + n;
    ssl->out_msgtype = MBEDTLS_SSL_MSG_HANDSHAKE;
    ssl->out_msg[0]  = MBEDTLS_SSL_HS_SERVER_KEY_EXCHANGE;

    ssl->state++;

    if( ( ret = mbedtls_ssl_write_record( ssl ) ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ssl_write_record", ret );
        return( ret );
    }

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= write server key exchange" ) );

    return( 0 );
}
