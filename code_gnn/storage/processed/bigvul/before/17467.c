ProcXvQueryAdaptors(ClientPtr client)
{
    xvFormat format;
    xvAdaptorInfo ainfo;
    xvQueryAdaptorsReply rep;
    int totalSize, na, nf, rc;
    int nameSize;
    XvAdaptorPtr pa;
    XvFormatPtr pf;
    WindowPtr pWin;
    ScreenPtr pScreen;
    XvScreenPtr pxvs;

    REQUEST(xvQueryAdaptorsReq);
    REQUEST_SIZE_MATCH(xvQueryAdaptorsReq);

    rc = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    pScreen = pWin->drawable.pScreen;
    pxvs = (XvScreenPtr) dixLookupPrivate(&pScreen->devPrivates,
                                          XvGetScreenKey());
    if (!pxvs) {
        rep = (xvQueryAdaptorsReply) {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = 0,
            .num_adaptors = 0
        };

        _WriteQueryAdaptorsReply(client, &rep);

        return Success;
    }

    rep = (xvQueryAdaptorsReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .num_adaptors = pxvs->nAdaptors
    };

     

    totalSize = pxvs->nAdaptors * sz_xvAdaptorInfo;

     

    na = pxvs->nAdaptors;
    pa = pxvs->pAdaptors;
    while (na--) {
        totalSize += pad_to_int32(strlen(pa->name));
        totalSize += pa->nFormats * sz_xvFormat;
        pa++;
    }

    rep.length = bytes_to_int32(totalSize);

    _WriteQueryAdaptorsReply(client, &rep);

    na = pxvs->nAdaptors;
    pa = pxvs->pAdaptors;
    while (na--) {

        ainfo.base_id = pa->base_id;
        ainfo.num_ports = pa->nPorts;
        ainfo.type = pa->type;
        ainfo.name_size = nameSize = strlen(pa->name);
        ainfo.num_formats = pa->nFormats;

        _WriteAdaptorInfo(client, &ainfo);

        WriteToClient(client, nameSize, pa->name);

        nf = pa->nFormats;
        pf = pa->pFormats;
        while (nf--) {
            format.depth = pf->depth;
            format.visual = pf->visual;
            _WriteFormat(client, &format);
            pf++;
        }

        pa++;

    }

    return Success;
}
