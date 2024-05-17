ProcXFixesCreateRegionFromGC(ClientPtr client)
{
    RegionPtr pRegion, pClip;
    GCPtr pGC;
    int rc;

    REQUEST(xXFixesCreateRegionFromGCReq);

    REQUEST_SIZE_MATCH(xXFixesCreateRegionFromGCReq);
    LEGAL_NEW_RESOURCE(stuff->region, client);

    rc = dixLookupGC(&pGC, stuff->gc, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    if (pGC->clientClip) {
        pClip = (RegionPtr) pGC->clientClip;
        pRegion = XFixesRegionCopy(pClip);
        if (!pRegion)
            return BadAlloc;
    } else {
        return BadMatch;
    }

    if (!AddResource(stuff->region, RegionResType, (void *) pRegion))
        return BadAlloc;

    return Success;
}
