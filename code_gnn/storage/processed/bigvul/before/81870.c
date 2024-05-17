int wc_ecc_encrypt(ecc_key* privKey, ecc_key* pubKey, const byte* msg,
                word32 msgSz, byte* out, word32* outSz, ecEncCtx* ctx)
{
    int          ret = 0;
    word32       blockSz;
    word32       digestSz;
    ecEncCtx     localCtx;
#ifdef WOLFSSL_SMALL_STACK
    byte*        sharedSecret;
    byte*        keys;
#else
    byte         sharedSecret[ECC_MAXSIZE];   
    byte         keys[ECC_BUFSIZE];          
#endif
    word32       sharedSz = ECC_MAXSIZE;
    int          keysLen;
    int          encKeySz;
    int          ivSz;
    int          offset = 0;          
    byte*        encKey;
    byte*        encIv;
    byte*        macKey;

    if (privKey == NULL || pubKey == NULL || msg == NULL || out == NULL ||
                           outSz  == NULL)
        return BAD_FUNC_ARG;

    if (ctx == NULL) {   
        ecc_ctx_init(&localCtx, 0);
        ctx = &localCtx;
    }

    ret = ecc_get_key_sizes(ctx, &encKeySz, &ivSz, &keysLen, &digestSz,
                            &blockSz);
    if (ret != 0)
        return ret;

    if (ctx->protocol == REQ_RESP_SERVER) {
        offset = keysLen;
        keysLen *= 2;

        if (ctx->srvSt != ecSRV_RECV_REQ)
            return BAD_STATE_E;

        ctx->srvSt = ecSRV_BAD_STATE;  
    }
    else if (ctx->protocol == REQ_RESP_CLIENT) {
        if (ctx->cliSt != ecCLI_SALT_SET)
            return BAD_STATE_E;

        ctx->cliSt = ecCLI_SENT_REQ;  
    }

    if (keysLen > ECC_BUFSIZE)  
        return BUFFER_E;

    if ( (msgSz%blockSz) != 0)
        return BAD_PADDING_E;

    if (*outSz < (msgSz + digestSz))
        return BUFFER_E;

#ifdef WOLFSSL_SMALL_STACK
    sharedSecret = (byte*)XMALLOC(ECC_MAXSIZE, NULL, DYNAMIC_TYPE_ECC_BUFFER);
    if (sharedSecret == NULL)
        return MEMORY_E;

    keys = (byte*)XMALLOC(ECC_BUFSIZE, NULL, DYNAMIC_TYPE_ECC_BUFFER);
    if (keys == NULL) {
        XFREE(sharedSecret, NULL, DYNAMIC_TYPE_ECC_BUFFER);
        return MEMORY_E;
    }
#endif

    do {
    #if defined(WOLFSSL_ASYNC_CRYPT)
        ret = wc_AsyncWait(ret, &privKey->asyncDev, WC_ASYNC_FLAG_CALL_AGAIN);
        if (ret != 0)
            break;
    #endif
        ret = wc_ecc_shared_secret(privKey, pubKey, sharedSecret, &sharedSz);
    } while (ret == WC_PENDING_E);
    if (ret == 0) {
       switch (ctx->kdfAlgo) {
           case ecHKDF_SHA256 :
               ret = wc_HKDF(WC_SHA256, sharedSecret, sharedSz, ctx->kdfSalt,
                          ctx->kdfSaltSz, ctx->kdfInfo, ctx->kdfInfoSz,
                          keys, keysLen);
               break;

           default:
               ret = BAD_FUNC_ARG;
               break;
       }
    }

    if (ret == 0) {
       encKey = keys + offset;
       encIv  = encKey + encKeySz;
       macKey = encKey + encKeySz + ivSz;

       switch (ctx->encAlgo) {
           case ecAES_128_CBC:
               {
                   Aes aes;
                   ret = wc_AesSetKey(&aes, encKey, KEY_SIZE_128, encIv,
                                                                AES_ENCRYPTION);
                   if (ret != 0)
                       break;
                   ret = wc_AesCbcEncrypt(&aes, out, msg, msgSz);
                #if defined(WOLFSSL_ASYNC_CRYPT)
                   ret = wc_AsyncWait(ret, &aes.asyncDev, WC_ASYNC_FLAG_NONE);
                #endif
               }
               break;

           default:
               ret = BAD_FUNC_ARG;
               break;
       }
    }

    if (ret == 0) {
       switch (ctx->macAlgo) {
           case ecHMAC_SHA256:
               {
                   Hmac hmac;
                   ret = wc_HmacInit(&hmac, NULL, INVALID_DEVID);
                   if (ret == 0) {
                       ret = wc_HmacSetKey(&hmac, WC_SHA256, macKey, WC_SHA256_DIGEST_SIZE);
                       if (ret == 0)
                           ret = wc_HmacUpdate(&hmac, out, msgSz);
                       if (ret == 0)
                           ret = wc_HmacUpdate(&hmac, ctx->macSalt, ctx->macSaltSz);
                       if (ret == 0)
                           ret = wc_HmacFinal(&hmac, out+msgSz);
                       wc_HmacFree(&hmac);
                   }
               }
               break;

           default:
               ret = BAD_FUNC_ARG;
               break;
       }
    }

    if (ret == 0)
       *outSz = msgSz + digestSz;

#ifdef WOLFSSL_SMALL_STACK
    XFREE(sharedSecret, NULL, DYNAMIC_TYPE_ECC_BUFFER);
    XFREE(keys, NULL, DYNAMIC_TYPE_ECC_BUFFER);
#endif

    return ret;
}