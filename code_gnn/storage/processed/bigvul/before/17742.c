ProcXF86DRIQueryDirectRenderingCapable(register ClientPtr client)
{
    xXF86DRIQueryDirectRenderingCapableReply rep;
    Bool isCapable;

    REQUEST(xXF86DRIQueryDirectRenderingCapableReq);
    REQUEST_SIZE_MATCH(xXF86DRIQueryDirectRenderingCapableReq);
    if (stuff->screen >= screenInfo.numScreens) {
        client->errorValue = stuff->screen;
        return BadValue;
    }

    if (!DRIQueryDirectRenderingCapable(screenInfo.screens[stuff->screen],
                                        &isCapable)) {
        return BadValue;
    }

    if (!client->local || client->swapped)
        isCapable = 0;

    rep = (xXF86DRIQueryDirectRenderingCapableReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .isCapable = isCapable
    };

    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
    }

    WriteToClient(client,
                  sizeof(xXF86DRIQueryDirectRenderingCapableReply),
                  &rep);
    return Success;
}
