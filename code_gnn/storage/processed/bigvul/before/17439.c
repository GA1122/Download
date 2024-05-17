InitConstructResourceBytesCtx(ConstructResourceBytesCtx *ctx,
                              ClientPtr                  sendClient,
                              long                       numSpecs,
                              xXResResourceIdSpec       *specs)
{
    ctx->sendClient = sendClient;
    ctx->numSizes = 0;
    ctx->resultBytes = 0;
    xorg_list_init(&ctx->response);
    ctx->status = Success;
    ctx->numSpecs = numSpecs;
    ctx->specs = specs;
    ctx->visitedResources = ht_create(sizeof(XID), 0,
                                      ht_resourceid_hash, ht_resourceid_compare,
                                      NULL);

    if (!ctx->visitedResources) {
        return FALSE;
    } else {
        return TRUE;
    }
}
