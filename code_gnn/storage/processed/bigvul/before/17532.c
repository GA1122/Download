ProcPseudoramiXDispatch(ClientPtr client)
{
    REQUEST(xReq);
    TRACE;
    switch (stuff->data) {
    case X_PanoramiXQueryVersion:
        return ProcPseudoramiXQueryVersion(client);

    case X_PanoramiXGetState:
        return ProcPseudoramiXGetState(client);

    case X_PanoramiXGetScreenCount:
        return ProcPseudoramiXGetScreenCount(client);

    case X_PanoramiXGetScreenSize:
        return ProcPseudoramiXGetScreenSize(client);

    case X_XineramaIsActive:
        return ProcPseudoramiXIsActive(client);

    case X_XineramaQueryScreens:
        return ProcPseudoramiXQueryScreens(client);
    }
    return BadRequest;
}
