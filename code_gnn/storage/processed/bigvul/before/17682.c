ProcXFixesCreateRegionFromPicture(ClientPtr client)
{
    RegionPtr pRegion;
    PicturePtr pPicture;

    REQUEST(xXFixesCreateRegionFromPictureReq);

    REQUEST_SIZE_MATCH(xXFixesCreateRegionFromPictureReq);
    LEGAL_NEW_RESOURCE(stuff->region, client);

    VERIFY_PICTURE(pPicture, stuff->picture, client, DixGetAttrAccess);

    if (!pPicture->pDrawable)
        return RenderErrBase + BadPicture;

    if (pPicture->clientClip) {
        pRegion = XFixesRegionCopy((RegionPtr) pPicture->clientClip);
        if (!pRegion)
            return BadAlloc;
    } else {
        return BadMatch;
    }

    if (!AddResource(stuff->region, RegionResType, (void *) pRegion))
        return BadAlloc;

    return Success;
}
