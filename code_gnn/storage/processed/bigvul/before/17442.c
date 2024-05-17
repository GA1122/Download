ProcXResQueryClientPixmapBytes(ClientPtr client)
{
    REQUEST(xXResQueryClientPixmapBytesReq);
    xXResQueryClientPixmapBytesReply rep;
    int clientID;
    unsigned long bytes;

    REQUEST_SIZE_MATCH(xXResQueryClientPixmapBytesReq);

    clientID = CLIENT_ID(stuff->xid);

    if ((clientID >= currentMaxClients) || !clients[clientID]) {
        client->errorValue = stuff->xid;
        return BadValue;
    }

    bytes = 0;

    FindAllClientResources(clients[clientID], ResFindResourcePixmaps,
                           (void *) (&bytes));

    rep = (xXResQueryClientPixmapBytesReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .bytes = bytes,
#ifdef _XSERVER64
        .bytes_overflow = bytes >> 32
#else
        .bytes_overflow = 0
#endif
    };
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.bytes);
        swapl(&rep.bytes_overflow);
    }
    WriteToClient(client, sizeof(xXResQueryClientPixmapBytesReply), &rep);

    return Success;
}
