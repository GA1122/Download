int wc_ecc_ctx_set_peer_salt(ecEncCtx* ctx, const byte* salt)
{
    byte tmp[EXCHANGE_SALT_SZ/2];
    int  halfSz = EXCHANGE_SALT_SZ/2;

    if (ctx == NULL || ctx->protocol == 0 || salt == NULL)
        return BAD_FUNC_ARG;

    if (ctx->protocol == REQ_RESP_CLIENT) {
        XMEMCPY(ctx->serverSalt, salt, EXCHANGE_SALT_SZ);
        if (ctx->cliSt == ecCLI_SALT_GET)
            ctx->cliSt =  ecCLI_SALT_SET;
        else {
            ctx->cliSt =  ecCLI_BAD_STATE;
            return BAD_STATE_E;
        }
    }
    else {
        XMEMCPY(ctx->clientSalt, salt, EXCHANGE_SALT_SZ);
        if (ctx->srvSt == ecSRV_SALT_GET)
            ctx->srvSt =  ecSRV_SALT_SET;
        else {
            ctx->srvSt =  ecSRV_BAD_STATE;
            return BAD_STATE_E;
        }
    }

     
     
    XMEMCPY(tmp, ctx->clientSalt + halfSz, halfSz);
    XMEMCPY(ctx->clientSalt + halfSz, ctx->serverSalt, halfSz);
    XMEMCPY(ctx->serverSalt, tmp, halfSz);

    ctx->kdfSalt   = ctx->clientSalt;
    ctx->kdfSaltSz = EXCHANGE_SALT_SZ;

    ctx->macSalt   = ctx->serverSalt;
    ctx->macSaltSz = EXCHANGE_SALT_SZ;

    if (ctx->kdfInfo == NULL) {
         
        ctx->kdfInfo   = (const byte*)exchange_info;
        ctx->kdfInfoSz = EXCHANGE_INFO_SZ;
    }

    return 0;
}