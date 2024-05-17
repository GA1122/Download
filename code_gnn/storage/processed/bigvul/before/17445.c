ProcXResQueryVersion(ClientPtr client)
{
    xXResQueryVersionReply rep = {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .server_major = SERVER_XRES_MAJOR_VERSION,
        .server_minor = SERVER_XRES_MINOR_VERSION
    };

    REQUEST_SIZE_MATCH(xXResQueryVersionReq);

    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swaps(&rep.server_major);
        swaps(&rep.server_minor);
    }
    WriteToClient(client, sizeof(xXResQueryVersionReply), &rep);
    return Success;
}
