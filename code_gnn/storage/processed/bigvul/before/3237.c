void aes_setkey_enc( aes_context *ctx, const unsigned char *key, int keysize )
{
    int i;
    unsigned long *RK;

#if !defined(XYSSL_AES_ROM_TABLES)
    if( aes_init_done == 0 )
    {
        aes_gen_tables();
        aes_init_done = 1;
    }
#endif

    switch( keysize )
    {
        case 128: ctx->nr = 10; break;
        case 192: ctx->nr = 12; break;
        case 256: ctx->nr = 14; break;
        default : return;
    }

#if defined(PADLOCK_ALIGN16)
    ctx->rk = RK = PADLOCK_ALIGN16( ctx->buf );
#else
    ctx->rk = RK = ctx->buf;
#endif

    for( i = 0; i < (keysize >> 5); i++ )
    {
        GET_ULONG_LE( RK[i], key, i << 2 );
    }

    switch( ctx->nr )
    {
        case 10:

            for( i = 0; i < 10; i++, RK += 4 )
            {
                RK[4]  = RK[0] ^ RCON[i] ^
                    ( FSb[ ( RK[3] >>  8 ) & 0xFF ]       ) ^
                    ( FSb[ ( RK[3] >> 16 ) & 0xFF ] <<  8 ) ^
                    ( FSb[ ( RK[3] >> 24 ) & 0xFF ] << 16 ) ^
                    ( ((unsigned int)FSb[ ( RK[3]       ) & 0xFF ]) << 24 );

                RK[5]  = RK[1] ^ RK[4];
                RK[6]  = RK[2] ^ RK[5];
                RK[7]  = RK[3] ^ RK[6];
            }
            break;

        case 12:

            for( i = 0; i < 8; i++, RK += 6 )
            {
                RK[6]  = RK[0] ^ RCON[i] ^
                    ( FSb[ ( RK[5] >>  8 ) & 0xFF ]       ) ^
                    ( FSb[ ( RK[5] >> 16 ) & 0xFF ] <<  8 ) ^
                    ( FSb[ ( RK[5] >> 24 ) & 0xFF ] << 16 ) ^
                    ( ((unsigned int)FSb[ ( RK[5]       ) & 0xFF ]) << 24 );

                RK[7]  = RK[1] ^ RK[6];
                RK[8]  = RK[2] ^ RK[7];
                RK[9]  = RK[3] ^ RK[8];
                RK[10] = RK[4] ^ RK[9];
                RK[11] = RK[5] ^ RK[10];
            }
            break;

        case 14:

            for( i = 0; i < 7; i++, RK += 8 )
            {
                RK[8]  = RK[0] ^ RCON[i] ^
                    ( FSb[ ( RK[7] >>  8 ) & 0xFF ]       ) ^
                    ( FSb[ ( RK[7] >> 16 ) & 0xFF ] <<  8 ) ^
                    ( FSb[ ( RK[7] >> 24 ) & 0xFF ] << 16 ) ^
                    ( ((unsigned int)FSb[ ( RK[7]       ) & 0xFF ]) << 24 );

                RK[9]  = RK[1] ^ RK[8];
                RK[10] = RK[2] ^ RK[9];
                RK[11] = RK[3] ^ RK[10];

                RK[12] = RK[4] ^
                    ( FSb[ ( RK[11]       ) & 0xFF ]       ) ^
                    ( FSb[ ( RK[11] >>  8 ) & 0xFF ] <<  8 ) ^
                    ( FSb[ ( RK[11] >> 16 ) & 0xFF ] << 16 ) ^
                    ( ((unsigned int)FSb[ ( RK[11] >> 24 ) & 0xFF ]) << 24 );

                RK[13] = RK[5] ^ RK[12];
                RK[14] = RK[6] ^ RK[13];
                RK[15] = RK[7] ^ RK[14];
            }
            break;

        default:

            break;
    }
}
