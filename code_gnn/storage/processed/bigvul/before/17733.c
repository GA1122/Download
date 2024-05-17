ProcXF86DRICreateContext(register ClientPtr client)
{
    xXF86DRICreateContextReply rep = {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0
    };
    ScreenPtr pScreen;

    REQUEST(xXF86DRICreateContextReq);
    REQUEST_SIZE_MATCH(xXF86DRICreateContextReq);
    if (stuff->screen >= screenInfo.numScreens) {
        client->errorValue = stuff->screen;
        return BadValue;
    }

    pScreen = screenInfo.screens[stuff->screen];

    if (!DRICreateContext(pScreen,
                          NULL,
                          stuff->context, (drm_context_t *) &rep.hHWContext)) {
        return BadValue;
    }

    WriteToClient(client, sizeof(xXF86DRICreateContextReply), &rep);
    return Success;
}
