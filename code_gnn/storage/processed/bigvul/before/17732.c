ProcXF86DRICloseConnection(register ClientPtr client)
{
    REQUEST(xXF86DRICloseConnectionReq);
    REQUEST_SIZE_MATCH(xXF86DRICloseConnectionReq);
    if (stuff->screen >= screenInfo.numScreens) {
        client->errorValue = stuff->screen;
        return BadValue;
    }

    DRICloseConnection(screenInfo.screens[stuff->screen]);

    return Success;
}
