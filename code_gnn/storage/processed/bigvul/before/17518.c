XineramaXvStopVideo(ClientPtr client)
{
    int result, i;
    PanoramiXRes *draw, *port;

    REQUEST(xvStopVideoReq);
    REQUEST_SIZE_MATCH(xvStopVideoReq);

    result = dixLookupResourceByClass((void **) &draw, stuff->drawable,
                                      XRC_DRAWABLE, client, DixWriteAccess);
    if (result != Success)
        return (result == BadValue) ? BadDrawable : result;

    result = dixLookupResourceByType((void **) &port, stuff->port,
                                     XvXRTPort, client, DixReadAccess);
    if (result != Success)
        return result;

    FOR_NSCREENS_BACKWARD(i) {
        if (port->info[i].id) {
            stuff->drawable = draw->info[i].id;
            stuff->port = port->info[i].id;
            result = ProcXvStopVideo(client);
        }
    }

    return result;
}
