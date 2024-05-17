AddSubResourceSizeSpec(void *value,
                       XID id,
                       RESTYPE type,
                       void *cdata)
{
    ConstructResourceBytesCtx *ctx = cdata;

    if (ctx->status == Success) {
        xXResResourceSizeSpec **prevCrossRef =
          ht_find(ctx->visitedSubResources, &value);
        if (!prevCrossRef) {
            Bool ok = TRUE;
            xXResResourceSizeSpec *crossRef =
                AddFragment(&ctx->response, sizeof(xXResResourceSizeSpec));
            ok = ok && crossRef != NULL;
            if (ok) {
                xXResResourceSizeSpec **p;
                p = ht_add(ctx->visitedSubResources, &value);
                if (!p) {
                    ok = FALSE;
                } else {
                    *p = crossRef;
                }
            }
            if (!ok) {
                ctx->status = BadAlloc;
            } else {
                SizeType sizeFunc = GetResourceTypeSizeFunc(type);
                ResourceSizeRec size = { 0, 0, 0 };
                sizeFunc(value, id, &size);

                crossRef->spec.resource = id;
                crossRef->spec.type = type;
                crossRef->bytes = size.resourceSize;
                crossRef->refCount = size.refCnt;
                crossRef->useCount = 1;

                ++ctx->sizeValue->numCrossReferences;

                ctx->resultBytes += sizeof(*crossRef);
            }
        } else {
             
            ++(*prevCrossRef)->useCount;
        }
    }
}
