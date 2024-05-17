int EVP_DecodeUpdate(EVP_ENCODE_CTX *ctx, unsigned char *out, int *outl,
                     const unsigned char *in, int inl)
{
    int seof = 0, eof = 0, rv = -1, ret = 0, i, v, tmp, n, decoded_len;
    unsigned char *d;

    n = ctx->num;
    d = ctx->enc_data;

    if (n > 0 && d[n - 1] == '=') {
        eof++;
        if (n > 1 && d[n - 2] == '=')
            eof++;
    }

      
    if (inl == 0) {
        rv = 0;
        goto end;
    }

    for (i = 0; i < inl; i++) {
        tmp = *(in++);
        v = conv_ascii2bin(tmp);
        if (v == B64_ERROR) {
            rv = -1;
            goto end;
        }

        if (tmp == '=') {
            eof++;
        } else if (eof > 0 && B64_BASE64(v)) {
             
            rv = -1;
            goto end;
        }

        if (eof > 2) {
            rv = -1;
            goto end;
        }

        if (v == B64_EOF) {
            seof = 1;
            goto tail;
        }

         
        if (B64_BASE64(v)) {
            if (n >= 64) {
                 
                rv = -1;
                goto end;
            }
            OPENSSL_assert(n < (int)sizeof(ctx->enc_data));
            d[n++] = tmp;
        }

        if (n == 64) {
            decoded_len = EVP_DecodeBlock(out, d, n);
            n = 0;
            if (decoded_len < 0 || eof > decoded_len) {
                rv = -1;
                goto end;
            }
            ret += decoded_len - eof;
            out += decoded_len - eof;
        }
    }

     
tail:
    if (n > 0) {
        if ((n & 3) == 0) {
            decoded_len = EVP_DecodeBlock(out, d, n);
            n = 0;
            if (decoded_len < 0 || eof > decoded_len) {
                rv = -1;
                goto end;
            }
            ret += (decoded_len - eof);
        } else if (seof) {
             
            rv = -1;
            goto end;
        }
    }

    rv = seof || (n == 0 && eof) ? 0 : 1;
end:
     
    *outl = ret;
    ctx->num = n;
    return (rv);
}
