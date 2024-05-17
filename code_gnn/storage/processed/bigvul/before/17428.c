AddResourceSizeValue(void *ptr, XID id, RESTYPE type, void *cdata)
{
    ConstructResourceBytesCtx *ctx = cdata;
    if (ctx->status == Success &&
        !ht_find(ctx->visitedResources, &id)) {
        Bool ok = TRUE;
        HashTable ht;
        HtGenericHashSetupRec htSetup = {
            .keySize = sizeof(void*)
        };

         

        xXResResourceSizeValue *value =
            AddFragment(&ctx->response, sizeof(xXResResourceSizeValue));
        if (!value) {
            ok = FALSE;
        }
        ok = ok && ht_add(ctx->visitedResources, &id);
        if (ok) {
            ht = ht_create(htSetup.keySize,
                           sizeof(xXResResourceSizeSpec*),
                           ht_generic_hash, ht_generic_compare,
                           &htSetup);
            ok = ok && ht;
        }

        if (!ok) {
            ctx->status = BadAlloc;
        } else {
            SizeType sizeFunc = GetResourceTypeSizeFunc(type);
            ResourceSizeRec size = { 0, 0, 0 };

            sizeFunc(ptr, id, &size);

            value->size.spec.resource = id;
            value->size.spec.type = type;
            value->size.bytes = size.resourceSize;
            value->size.refCount = size.refCnt;
            value->size.useCount = 1;
            value->numCrossReferences = 0;

            ctx->sizeValue = value;
            ctx->visitedSubResources = ht;
            FindSubResources(ptr, type, AddSubResourceSizeSpec, ctx);
            ctx->visitedSubResources = NULL;
            ctx->sizeValue = NULL;

            ctx->resultBytes += sizeof(*value);
            ++ctx->numSizes;

            ht_destroy(ht);
        }
    }
}
