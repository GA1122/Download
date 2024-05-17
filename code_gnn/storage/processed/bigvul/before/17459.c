ProcXvGetPortAttribute(ClientPtr client)
{
    INT32 value;
    int status;
    XvPortPtr pPort;
    xvGetPortAttributeReply rep;

    REQUEST(xvGetPortAttributeReq);
    REQUEST_SIZE_MATCH(xvGetPortAttributeReq);

    VALIDATE_XV_PORT(stuff->port, pPort, DixGetAttrAccess);

    if (!ValidAtom(stuff->attribute)) {
        client->errorValue = stuff->attribute;
        return BadAtom;
    }

    status = XvdiGetPortAttribute(client, pPort, stuff->attribute, &value);
    if (status != Success) {
        client->errorValue = stuff->attribute;
        return status;
    }

    rep = (xvGetPortAttributeReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .value = value
    };

    _WriteGetPortAttributeReply(client, &rep);

    return Success;
}
