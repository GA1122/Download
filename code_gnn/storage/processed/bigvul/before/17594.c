ProcRenderQueryVersion(ClientPtr client)
{
    RenderClientPtr pRenderClient = GetRenderClient(client);
    xRenderQueryVersionReply rep = {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0
    };

    REQUEST(xRenderQueryVersionReq);

    REQUEST_SIZE_MATCH(xRenderQueryVersionReq);

    pRenderClient->major_version = stuff->majorVersion;
    pRenderClient->minor_version = stuff->minorVersion;

    if ((stuff->majorVersion * 1000 + stuff->minorVersion) <
        (SERVER_RENDER_MAJOR_VERSION * 1000 + SERVER_RENDER_MINOR_VERSION)) {
        rep.majorVersion = stuff->majorVersion;
        rep.minorVersion = stuff->minorVersion;
    }
    else {
        rep.majorVersion = SERVER_RENDER_MAJOR_VERSION;
        rep.minorVersion = SERVER_RENDER_MINOR_VERSION;
    }

    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.majorVersion);
        swapl(&rep.minorVersion);
    }
    WriteToClient(client, sizeof(xRenderQueryVersionReply), &rep);
    return Success;
}
