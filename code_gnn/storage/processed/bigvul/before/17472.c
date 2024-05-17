ProcXvQueryPortAttributes(ClientPtr client)
{
    int size, i;
    XvPortPtr pPort;
    XvAttributePtr pAtt;
    xvQueryPortAttributesReply rep;
    xvAttributeInfo Info;

    REQUEST(xvQueryPortAttributesReq);
    REQUEST_SIZE_MATCH(xvQueryPortAttributesReq);

    VALIDATE_XV_PORT(stuff->port, pPort, DixGetAttrAccess);

    rep = (xvQueryPortAttributesReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .num_attributes = pPort->pAdaptor->nAttributes,
        .text_size = 0
    };

    for (i = 0, pAtt = pPort->pAdaptor->pAttributes;
         i < pPort->pAdaptor->nAttributes; i++, pAtt++) {
        rep.text_size += pad_to_int32(strlen(pAtt->name) + 1);
    }

    rep.length = (pPort->pAdaptor->nAttributes * sz_xvAttributeInfo)
        + rep.text_size;
    rep.length >>= 2;

    _WriteQueryPortAttributesReply(client, &rep);

    for (i = 0, pAtt = pPort->pAdaptor->pAttributes;
         i < pPort->pAdaptor->nAttributes; i++, pAtt++) {
        size = strlen(pAtt->name) + 1;   
        Info.flags = pAtt->flags;
        Info.min = pAtt->min_value;
        Info.max = pAtt->max_value;
        Info.size = pad_to_int32(size);

        _WriteAttributeInfo(client, &Info);

        WriteToClient(client, size, pAtt->name);
    }

    return Success;
}
