ProcXF86DRIGetDeviceInfo(register ClientPtr client)
{
    xXF86DRIGetDeviceInfoReply rep = {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0
    };
    drm_handle_t hFrameBuffer;
    void *pDevPrivate;

    REQUEST(xXF86DRIGetDeviceInfoReq);
    REQUEST_SIZE_MATCH(xXF86DRIGetDeviceInfoReq);
    if (stuff->screen >= screenInfo.numScreens) {
        client->errorValue = stuff->screen;
        return BadValue;
    }

    if (!DRIGetDeviceInfo(screenInfo.screens[stuff->screen],
                          &hFrameBuffer,
                          (int *) &rep.framebufferOrigin,
                          (int *) &rep.framebufferSize,
                          (int *) &rep.framebufferStride,
                          (int *) &rep.devPrivateSize, &pDevPrivate)) {
        return BadValue;
    }

    rep.hFrameBufferLow = (CARD32) (hFrameBuffer & 0xffffffff);
#if defined(LONG64) && !defined(__linux__)
    rep.hFrameBufferHigh = (CARD32) (hFrameBuffer >> 32);
#else
    rep.hFrameBufferHigh = 0;
#endif

    if (rep.devPrivateSize) {
        rep.length = bytes_to_int32(SIZEOF(xXF86DRIGetDeviceInfoReply) -
                                    SIZEOF(xGenericReply) +
                                    pad_to_int32(rep.devPrivateSize));
    }

    WriteToClient(client, sizeof(xXF86DRIGetDeviceInfoReply), &rep);
    if (rep.length) {
        WriteToClient(client, rep.devPrivateSize, pDevPrivate);
    }
    return Success;
}
