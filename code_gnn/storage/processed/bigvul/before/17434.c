ConstructResourceBytesByResource(XID aboutClient, ConstructResourceBytesCtx *ctx)
{
    int specIdx;
    for (specIdx = 0; specIdx < ctx->numSpecs; ++specIdx) {
        xXResResourceIdSpec *spec = ctx->specs + specIdx;
        if (spec->resource) {
            int cid = CLIENT_ID(spec->resource);
            if (cid < currentMaxClients &&
                (aboutClient == None || cid == aboutClient)) {
                ClientPtr client = clients[cid];
                if (client) {
                    ctx->curSpec = spec;
                    FindAllClientResources(client,
                                           AddResourceSizeValueByResource,
                                           ctx);
                }
            }
        }
    }
}
