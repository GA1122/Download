ProcXvQueryBestSize(ClientPtr client)
{
    unsigned int actual_width, actual_height;
    XvPortPtr pPort;
    xvQueryBestSizeReply rep;

    REQUEST(xvQueryBestSizeReq);
    REQUEST_SIZE_MATCH(xvQueryBestSizeReq);

    VALIDATE_XV_PORT(stuff->port, pPort, DixReadAccess);

    (*pPort->pAdaptor->ddQueryBestSize) (pPort, stuff->motion,
                                         stuff->vid_w, stuff->vid_h,
                                         stuff->drw_w, stuff->drw_h,
                                         &actual_width, &actual_height);

    rep = (xvQueryBestSizeReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .actual_width = actual_width,
        .actual_height = actual_height
    };

    _WriteQueryBestSizeReply(client, &rep);

    return Success;
}
