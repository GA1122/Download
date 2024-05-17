ProcXvListImageFormats(ClientPtr client)
{
    XvPortPtr pPort;
    XvImagePtr pImage;
    int i;
    xvListImageFormatsReply rep;
    xvImageFormatInfo info;

    REQUEST(xvListImageFormatsReq);

    REQUEST_SIZE_MATCH(xvListImageFormatsReq);

    VALIDATE_XV_PORT(stuff->port, pPort, DixReadAccess);

    rep = (xvListImageFormatsReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .num_formats = pPort->pAdaptor->nImages,
        .length =
            bytes_to_int32(pPort->pAdaptor->nImages * sz_xvImageFormatInfo)
    };

    _WriteListImageFormatsReply(client, &rep);

    pImage = pPort->pAdaptor->pImages;

    for (i = 0; i < pPort->pAdaptor->nImages; i++, pImage++) {
        info.id = pImage->id;
        info.type = pImage->type;
        info.byte_order = pImage->byte_order;
        memcpy(&info.guid, pImage->guid, 16);
        info.bpp = pImage->bits_per_pixel;
        info.num_planes = pImage->num_planes;
        info.depth = pImage->depth;
        info.red_mask = pImage->red_mask;
        info.green_mask = pImage->green_mask;
        info.blue_mask = pImage->blue_mask;
        info.format = pImage->format;
        info.y_sample_bits = pImage->y_sample_bits;
        info.u_sample_bits = pImage->u_sample_bits;
        info.v_sample_bits = pImage->v_sample_bits;
        info.horz_y_period = pImage->horz_y_period;
        info.horz_u_period = pImage->horz_u_period;
        info.horz_v_period = pImage->horz_v_period;
        info.vert_y_period = pImage->vert_y_period;
        info.vert_u_period = pImage->vert_u_period;
        info.vert_v_period = pImage->vert_v_period;
        memcpy(&info.comp_order, pImage->component_order, 32);
        info.scanline_order = pImage->scanline_order;
        _WriteImageFormatInfo(client, &info);
    }

    return Success;
}
