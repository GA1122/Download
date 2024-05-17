ProcXF86DRIDestroyDrawable(register ClientPtr client)
{
    REQUEST(xXF86DRIDestroyDrawableReq);
    DrawablePtr pDrawable;
    int rc;

    REQUEST_SIZE_MATCH(xXF86DRIDestroyDrawableReq);

    if (stuff->screen >= screenInfo.numScreens) {
        client->errorValue = stuff->screen;
        return BadValue;
    }

    rc = dixLookupDrawable(&pDrawable, stuff->drawable, client, 0,
                           DixReadAccess);
    if (rc != Success)
        return rc;

    if (!DRIDestroyDrawable(screenInfo.screens[stuff->screen], client,
                            pDrawable)) {
        return BadValue;
    }

    return Success;
}
