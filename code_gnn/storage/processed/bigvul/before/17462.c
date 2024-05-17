ProcXvGrabPort(ClientPtr client)
{
    int result, status;
    XvPortPtr pPort;
    xvGrabPortReply rep;

    REQUEST(xvGrabPortReq);
    REQUEST_SIZE_MATCH(xvGrabPortReq);

    VALIDATE_XV_PORT(stuff->port, pPort, DixReadAccess);

    status = XvdiGrabPort(client, pPort, stuff->time, &result);

    if (status != Success) {
        return status;
    }
    rep = (xvGrabPortReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .result = result
    };

    _WriteGrabPortReply(client, &rep);

    return Success;
}
