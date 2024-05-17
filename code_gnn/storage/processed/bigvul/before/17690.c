ProcXFixesQueryVersion(ClientPtr client)
{
    XFixesClientPtr pXFixesClient = GetXFixesClient(client);
    xXFixesQueryVersionReply rep = {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0
    };

    REQUEST(xXFixesQueryVersionReq);

    REQUEST_SIZE_MATCH(xXFixesQueryVersionReq);

    if (version_compare(stuff->majorVersion, stuff->minorVersion,
                        SERVER_XFIXES_MAJOR_VERSION,
                        SERVER_XFIXES_MINOR_VERSION) < 0) {
        rep.majorVersion = stuff->majorVersion;
        rep.minorVersion = stuff->minorVersion;
    }
    else {
        rep.majorVersion = SERVER_XFIXES_MAJOR_VERSION;
        rep.minorVersion = SERVER_XFIXES_MINOR_VERSION;
    }

    pXFixesClient->major_version = rep.majorVersion;
    pXFixesClient->minor_version = rep.minorVersion;
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.majorVersion);
        swapl(&rep.minorVersion);
    }
    WriteToClient(client, sizeof(xXFixesQueryVersionReply), &rep);
    return Success;
}
