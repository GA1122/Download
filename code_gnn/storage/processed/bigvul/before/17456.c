WillConstructMask(ClientPtr client, CARD32 mask,
                  ConstructClientIdCtx *ctx, int sendMask)
{
    if ((!mask || (mask & sendMask))
        && !(ctx->sentClientMasks[client->index] & sendMask)) {
        ctx->sentClientMasks[client->index] |= sendMask;
        return TRUE;
    } else {
        return FALSE;
    }
}
