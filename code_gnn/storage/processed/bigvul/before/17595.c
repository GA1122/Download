ProcRenderReferenceGlyphSet(ClientPtr client)
{
    GlyphSetPtr glyphSet;
    int rc;

    REQUEST(xRenderReferenceGlyphSetReq);

    REQUEST_SIZE_MATCH(xRenderReferenceGlyphSetReq);

    LEGAL_NEW_RESOURCE(stuff->gsid, client);

    rc = dixLookupResourceByType((void **) &glyphSet, stuff->existing,
                                 GlyphSetType, client, DixGetAttrAccess);
    if (rc != Success) {
        client->errorValue = stuff->existing;
        return rc;
    }
    glyphSet->refcnt++;
    if (!AddResource(stuff->gsid, GlyphSetType, (void *) glyphSet))
        return BadAlloc;
    return Success;
}
