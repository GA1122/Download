SProcPseudoramiXDispatch(ClientPtr client)
{
    REQUEST(xReq);

    TRACE;

    switch (stuff->data) {
    case X_PanoramiXQueryVersion:
        return SProcPseudoramiXQueryVersion(client);

    case X_PanoramiXGetState:
        return SProcPseudoramiXGetState(client);

    case X_PanoramiXGetScreenCount:
        return SProcPseudoramiXGetScreenCount(client);

    case X_PanoramiXGetScreenSize:
        return SProcPseudoramiXGetScreenSize(client);

    case X_XineramaIsActive:
        return SProcPseudoramiXIsActive(client);

    case X_XineramaQueryScreens:
        return SProcPseudoramiXQueryScreens(client);
    }
    return BadRequest;
}
