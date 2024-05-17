ConstructClientIdValue(ClientPtr sendClient, ClientPtr client, CARD32 mask,
                       ConstructClientIdCtx *ctx)
{
    xXResClientIdValue rep;

    rep.spec.client = client->clientAsMask;
    if (client->swapped) {
        swapl (&rep.spec.client);
    }

    if (WillConstructMask(client, mask, ctx, X_XResClientXIDMask)) {
        void *ptr = AddFragment(&ctx->response, sizeof(rep));
        if (!ptr) {
            return FALSE;
        }

        rep.spec.mask = X_XResClientXIDMask;
        rep.length = 0;
        if (sendClient->swapped) {
            swapl (&rep.spec.mask);
             
        }

        memcpy(ptr, &rep, sizeof(rep));

        ctx->resultBytes += sizeof(rep);
        ++ctx->numIds;
    }
    if (WillConstructMask(client, mask, ctx, X_XResLocalClientPIDMask)) {
        pid_t pid = GetClientPid(client);

        if (pid != -1) {
            void *ptr = AddFragment(&ctx->response,
                                    sizeof(rep) + sizeof(CARD32));
            CARD32 *value = (void*) ((char*) ptr + sizeof(rep));

            if (!ptr) {
                return FALSE;
            }

            rep.spec.mask = X_XResLocalClientPIDMask;
            rep.length = 4;

            if (sendClient->swapped) {
                swapl (&rep.spec.mask);
                swapl (&rep.length);
            }

            if (sendClient->swapped) {
                swapl (value);
            }
            memcpy(ptr, &rep, sizeof(rep));
            *value = pid;

            ctx->resultBytes += sizeof(rep) + sizeof(CARD32);
            ++ctx->numIds;
        }
    }

     
    return TRUE;
}
