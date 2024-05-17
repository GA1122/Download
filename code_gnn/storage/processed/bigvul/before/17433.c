ConstructClientResourceBytes(ClientPtr aboutClient,
                             ConstructResourceBytesCtx *ctx)
{
    int specIdx;
    for (specIdx = 0; specIdx < ctx->numSpecs; ++specIdx) {
        xXResResourceIdSpec* spec = ctx->specs + specIdx;
        if (spec->resource) {
             
        } else if (spec->type) {
            ctx->resType = spec->type;
            FindClientResourcesByType(aboutClient, spec->type,
                                      AddResourceSizeValueWithResType, ctx);
        } else {
            FindAllClientResources(aboutClient, AddResourceSizeValue, ctx);
        }
    }
}
