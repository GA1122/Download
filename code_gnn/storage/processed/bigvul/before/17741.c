ProcXF86DRIOpenConnection(register ClientPtr client)
{
    xXF86DRIOpenConnectionReply rep;
    drm_handle_t hSAREA;
    char *busIdString;
    CARD32 busIdStringLength = 0;

    REQUEST(xXF86DRIOpenConnectionReq);
    REQUEST_SIZE_MATCH(xXF86DRIOpenConnectionReq);
    if (stuff->screen >= screenInfo.numScreens) {
        client->errorValue = stuff->screen;
        return BadValue;
    }

    if (!DRIOpenConnection(screenInfo.screens[stuff->screen],
                           &hSAREA, &busIdString)) {
        return BadValue;
    }

    if (busIdString)
        busIdStringLength = strlen(busIdString);

    rep = (xXF86DRIOpenConnectionReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = bytes_to_int32(SIZEOF(xXF86DRIOpenConnectionReply) -
                                 SIZEOF(xGenericReply) +
                                 pad_to_int32(busIdStringLength)),
        .busIdStringLength = busIdStringLength,

        .hSAREALow = (CARD32) (hSAREA & 0xffffffff),
#if defined(LONG64) && !defined(__linux__)
        .hSAREAHigh = (CARD32) (hSAREA >> 32),
#else
        .hSAREAHigh = 0
#endif
    };

    WriteToClient(client, sizeof(xXF86DRIOpenConnectionReply), &rep);
    if (busIdStringLength)
        WriteToClient(client, busIdStringLength, busIdString);
    return Success;
}
