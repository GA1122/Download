ProcXF86DRIAuthConnection(register ClientPtr client)
{
    xXF86DRIAuthConnectionReply rep = {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .authenticated = 1
    };

    REQUEST(xXF86DRIAuthConnectionReq);
    REQUEST_SIZE_MATCH(xXF86DRIAuthConnectionReq);
    if (stuff->screen >= screenInfo.numScreens) {
        client->errorValue = stuff->screen;
        return BadValue;
    }

    if (!DRIAuthConnection(screenInfo.screens[stuff->screen], stuff->magic)) {
        ErrorF("Failed to authenticate %lu\n", (unsigned long) stuff->magic);
        rep.authenticated = 0;
    }
    WriteToClient(client, sizeof(xXF86DRIAuthConnectionReply), &rep);
    return Success;
}
