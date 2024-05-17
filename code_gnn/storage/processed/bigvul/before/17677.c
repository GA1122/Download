PanoramiXFixesSetPictureClipRegion(ClientPtr client)
{
    REQUEST(xXFixesSetPictureClipRegionReq);
    int result = Success, j;
    PanoramiXRes *pict;
    RegionPtr reg = NULL;

    REQUEST_SIZE_MATCH(xXFixesSetPictureClipRegionReq);

    if ((result = dixLookupResourceByType((void **) &pict, stuff->picture,
                                          XRT_PICTURE, client,
                                          DixWriteAccess))) {
        client->errorValue = stuff->picture;
        return result;
    }

    if (pict->u.pict.root)
        VERIFY_REGION_OR_NONE(reg, stuff->region, client, DixReadAccess);

    FOR_NSCREENS_BACKWARD(j) {
        ScreenPtr screen = screenInfo.screens[j];
        stuff->picture = pict->info[j].id;

        if (reg)
            RegionTranslate(reg, -screen->x, -screen->y);

        result =
            (*PanoramiXSaveXFixesVector[X_XFixesSetPictureClipRegion]) (client);

        if (reg)
            RegionTranslate(reg, screen->x, screen->y);

        if (result != Success)
            break;
    }

    return result;
}
