XineramaXvSetPortAttribute(ClientPtr client)
{
    REQUEST(xvSetPortAttributeReq);
    PanoramiXRes *port;
    int result, i;

    REQUEST_SIZE_MATCH(xvSetPortAttributeReq);

    result = dixLookupResourceByType((void **) &port, stuff->port,
                                     XvXRTPort, client, DixReadAccess);
    if (result != Success)
        return result;

    FOR_NSCREENS_BACKWARD(i) {
        if (port->info[i].id) {
            stuff->port = port->info[i].id;
            result = ProcXvSetPortAttribute(client);
        }
    }
    return result;
}
