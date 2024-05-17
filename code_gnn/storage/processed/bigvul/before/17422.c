ScreenSaverUnsetAttributes(ClientPtr client)
{
    REQUEST(xScreenSaverSetAttributesReq);
    DrawablePtr pDraw;
    ScreenSaverScreenPrivatePtr pPriv;
    int rc;

    REQUEST_SIZE_MATCH(xScreenSaverUnsetAttributesReq);
    rc = dixLookupDrawable(&pDraw, stuff->drawable, client, 0,
                           DixGetAttrAccess);
    if (rc != Success)
        return rc;
    pPriv = GetScreenPrivate(pDraw->pScreen);
    if (pPriv && pPriv->attr && pPriv->attr->client == client) {
        FreeResource(pPriv->attr->resource, AttrType);
        FreeScreenAttr(pPriv->attr);
        pPriv->attr = NULL;
        CheckScreenPrivate(pDraw->pScreen);
    }
    return Success;
}
