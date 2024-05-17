ProcPanoramiXDispatch(ClientPtr client)
{
    REQUEST(xReq);
    switch (stuff->data) {
    case X_PanoramiXQueryVersion:
        return ProcPanoramiXQueryVersion(client);
    case X_PanoramiXGetState:
        return ProcPanoramiXGetState(client);
    case X_PanoramiXGetScreenCount:
        return ProcPanoramiXGetScreenCount(client);
    case X_PanoramiXGetScreenSize:
        return ProcPanoramiXGetScreenSize(client);
    case X_XineramaIsActive:
        return ProcXineramaIsActive(client);
    case X_XineramaQueryScreens:
        return ProcXineramaQueryScreens(client);
    }
    return BadRequest;
}
