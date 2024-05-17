ProcXFixesCreateRegionFromBitmap(ClientPtr client)
{
    RegionPtr pRegion;
    PixmapPtr pPixmap;
    int rc;

    REQUEST(xXFixesCreateRegionFromBitmapReq);

    REQUEST_SIZE_MATCH(xXFixesCreateRegionFromBitmapReq);
    LEGAL_NEW_RESOURCE(stuff->region, client);

    rc = dixLookupResourceByType((void **) &pPixmap, stuff->bitmap, RT_PIXMAP,
                                 client, DixReadAccess);
    if (rc != Success) {
        client->errorValue = stuff->bitmap;
        return rc;
    }
    if (pPixmap->drawable.depth != 1)
        return BadMatch;

    pRegion = BitmapToRegion(pPixmap->drawable.pScreen, pPixmap);

    if (!pRegion)
        return BadAlloc;

    if (!AddResource(stuff->region, RegionResType, (void *) pRegion))
        return BadAlloc;

    return Success;
}
