void aes_crypt_cfb( aes_context *ctx,
                    int mode,
                    int length,
                    int *iv_off,
                    unsigned char iv[16],
                    const unsigned char *input,
                    unsigned char *output )
{
    int c, n = *iv_off;

    if( mode == AES_DECRYPT )
    {
        while( length-- )
        {
            if( n == 0 )
                aes_crypt_ecb( ctx, AES_ENCRYPT, iv, iv );

            c = *input++;
            *output++ = (unsigned char)( c ^ iv[n] );
            iv[n] = (unsigned char) c;

            n = (n + 1) & 0x0F;
        }
    }
    else
    {
        while( length-- )
        {
            if( n == 0 )
                aes_crypt_ecb( ctx, AES_ENCRYPT, iv, iv );

            iv[n] = *output++ = (unsigned char)( iv[n] ^ *input++ );

            n = (n + 1) & 0x0F;
        }
    }

    *iv_off = n;
}
