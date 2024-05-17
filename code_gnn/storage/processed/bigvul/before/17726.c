DGAStealMotionEvent(DeviceIntPtr dev, int index, int dx, int dy)
{
    DGAScreenPtr pScreenPriv;
    DGAEvent event;

    if (!DGAScreenKeyRegistered)         
        return FALSE;

    pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

    if (!pScreenPriv || !pScreenPriv->grabMouse)         
        return FALSE;

    event = (DGAEvent) {
        .header = ET_Internal,
        .type = ET_DGAEvent,
        .length = sizeof(event),
        .time = GetTimeInMillis(),
        .subtype = ET_Motion,
        .detail = 0,
        .dx = dx,
        .dy = dy
    };
    mieqEnqueue(dev, (InternalEvent *) &event);
    return TRUE;
}
