ProcXF86DRIDestroyContext(register ClientPtr client)
{
    REQUEST(xXF86DRIDestroyContextReq);
    REQUEST_SIZE_MATCH(xXF86DRIDestroyContextReq);
    if (stuff->screen >= screenInfo.numScreens) {
        client->errorValue = stuff->screen;
        return BadValue;
    }

    if (!DRIDestroyContext(screenInfo.screens[stuff->screen], stuff->context)) {
        return BadValue;
    }

    return Success;
}
