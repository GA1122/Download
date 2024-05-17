int aes_self_test( int verbose )
{
    int i, j, u, v, offset;
    unsigned char key[32];
    unsigned char buf[16];
    unsigned char prv[16];
    unsigned char iv[16];
    aes_context ctx;

    memset( key, 0, 32 );

     
    for( i = 0; i < 6; i++ )
    {
        u = i >> 1;
        v = i  & 1;

        if( verbose != 0 )
            printf( "  AES-ECB-%3d (%s): ", 128 + u * 64,
                    ( v == AES_DECRYPT ) ? "dec" : "enc" );

        memset( buf, 0, 16 );

        if( v == AES_DECRYPT )
        {
            aes_setkey_dec( &ctx, key, 128 + u * 64 );

            for( j = 0; j < 10000; j++ )
                aes_crypt_ecb( &ctx, v, buf, buf );

            if( memcmp( buf, aes_test_ecb_dec[u], 16 ) != 0 )
            {
                if( verbose != 0 )
                    printf( "failed\n" );

                return( 1 );
            }
        }
        else
        {
            aes_setkey_enc( &ctx, key, 128 + u * 64 );

            for( j = 0; j < 10000; j++ )
                aes_crypt_ecb( &ctx, v, buf, buf );

            if( memcmp( buf, aes_test_ecb_enc[u], 16 ) != 0 )
            {
                if( verbose != 0 )
                    printf( "failed\n" );

                return( 1 );
            }
        }

        if( verbose != 0 )
            printf( "passed\n" );
    }

    if( verbose != 0 )
        printf( "\n" );

     
    for( i = 0; i < 6; i++ )
    {
        u = i >> 1;
        v = i  & 1;

        if( verbose != 0 )
            printf( "  AES-CBC-%3d (%s): ", 128 + u * 64,
                    ( v == AES_DECRYPT ) ? "dec" : "enc" );

        memset( iv , 0, 16 );
        memset( prv, 0, 16 );
        memset( buf, 0, 16 );

        if( v == AES_DECRYPT )
        {
            aes_setkey_dec( &ctx, key, 128 + u * 64 );

            for( j = 0; j < 10000; j++ )
                aes_crypt_cbc( &ctx, v, 16, iv, buf, buf );

            if( memcmp( buf, aes_test_cbc_dec[u], 16 ) != 0 )
            {
                if( verbose != 0 )
                    printf( "failed\n" );

                return( 1 );
            }
        }
        else
        {
            aes_setkey_enc( &ctx, key, 128 + u * 64 );

            for( j = 0; j < 10000; j++ )
            {
                unsigned char tmp[16];

                aes_crypt_cbc( &ctx, v, 16, iv, buf, buf );

                memcpy( tmp, prv, 16 );
                memcpy( prv, buf, 16 );
                memcpy( buf, tmp, 16 );
            }

            if( memcmp( prv, aes_test_cbc_enc[u], 16 ) != 0 )
            {
                if( verbose != 0 )
                    printf( "failed\n" );

                return( 1 );
            }
        }

        if( verbose != 0 )
            printf( "passed\n" );
    }

    if( verbose != 0 )
        printf( "\n" );

     
    for( i = 0; i < 6; i++ )
    {
        u = i >> 1;
        v = i  & 1;

        if( verbose != 0 )
            printf( "  AES-CFB-%3d (%s): ", 128 + u * 64,
                    ( v == AES_DECRYPT ) ? "dec" : "enc" );

        memset( iv , 0, 16 );
        memset( buf, 0, 16 );
        offset = 0;

        if( v == AES_DECRYPT )
        {
            aes_setkey_dec( &ctx, key, 128 + u * 64 );

            for( j = 0; j < 10000; j++ )
                aes_crypt_cfb( &ctx, v, 16, &offset, iv, buf, buf );

            if( memcmp( buf, aes_test_cfb_dec[u], 16 ) != 0 )
            {
                if( verbose != 0 )
                    printf( "failed\n" );

                return( 1 );
            }
        }
        else
        {
            aes_setkey_enc( &ctx, key, 128 + u * 64 );

            for( j = 0; j < 10000; j++ )
                aes_crypt_cfb( &ctx, v, 16, &offset, iv, buf, buf );

            if( memcmp( buf, aes_test_cfb_enc[u], 16 ) != 0 )
            {
                if( verbose != 0 )
                    printf( "failed\n" );

                return( 1 );
            }
        }

        if( verbose != 0 )
            printf( "passed\n" );
    }

    if( verbose != 0 )
        printf( "\n" );

    return( 0 );
}
