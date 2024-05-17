ProcXF86DRICreateDrawable(ClientPtr client)
{
    xXF86DRICreateDrawableReply rep = {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0
    };
    DrawablePtr pDrawable;
    int rc;

    REQUEST(xXF86DRICreateDrawableReq);
    REQUEST_SIZE_MATCH(xXF86DRICreateDrawableReq);
    if (stuff->screen >= screenInfo.numScreens) {
        client->errorValue = stuff->screen;
        return BadValue;
    }

    rc = dixLookupDrawable(&pDrawable, stuff->drawable, client, 0,
                           DixReadAccess);
    if (rc != Success)
        return rc;

    if (!DRICreateDrawable(screenInfo.screens[stuff->screen], client,
                           pDrawable, (drm_drawable_t *) &rep.hHWDrawable)) {
        return BadValue;
    }

    WriteToClient(client, sizeof(xXF86DRICreateDrawableReply), &rep);
    return Success;
}
