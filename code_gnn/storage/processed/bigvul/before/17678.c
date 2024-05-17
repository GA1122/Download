PanoramiXFixesSetWindowShapeRegion(ClientPtr client)
{
    int result = Success, j;
    PanoramiXRes *win;
    RegionPtr reg = NULL;

    REQUEST(xXFixesSetWindowShapeRegionReq);

    REQUEST_SIZE_MATCH(xXFixesSetWindowShapeRegionReq);

    if ((result = dixLookupResourceByType((void **) &win, stuff->dest,
                                          XRT_WINDOW, client,
                                          DixWriteAccess))) {
        client->errorValue = stuff->dest;
        return result;
    }

    if (win->u.win.root)
        VERIFY_REGION_OR_NONE(reg, stuff->region, client, DixReadAccess);

    FOR_NSCREENS_FORWARD(j) {
        ScreenPtr screen = screenInfo.screens[j];
        stuff->dest = win->info[j].id;

        if (reg)
            RegionTranslate(reg, -screen->x, -screen->y);

        result =
            (*PanoramiXSaveXFixesVector[X_XFixesSetWindowShapeRegion]) (client);

        if (reg)
            RegionTranslate(reg, screen->x, screen->y);

        if (result != Success)
            break;
    }

    return result;
}
