ProcXF86DRIQueryVersion(register ClientPtr client)
{
    xXF86DRIQueryVersionReply rep = {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .majorVersion = SERVER_XF86DRI_MAJOR_VERSION,
        .minorVersion = SERVER_XF86DRI_MINOR_VERSION,
        .patchVersion = SERVER_XF86DRI_PATCH_VERSION
    };

    REQUEST_SIZE_MATCH(xXF86DRIQueryVersionReq);
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swaps(&rep.majorVersion);
        swaps(&rep.minorVersion);
        swapl(&rep.patchVersion);
    }
    WriteToClient(client, sizeof(xXF86DRIQueryVersionReply), &rep);
    return Success;
}
