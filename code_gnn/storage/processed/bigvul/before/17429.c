AddResourceSizeValueByResource(void *ptr, XID id, RESTYPE type, void *cdata)
{
    ConstructResourceBytesCtx *ctx = cdata;
    xXResResourceIdSpec *spec = ctx->curSpec;

    if ((!spec->type || spec->type == type) &&
        (!spec->resource || spec->resource == id)) {
        AddResourceSizeValue(ptr, id, type, ctx);
    }
}
