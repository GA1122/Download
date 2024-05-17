DGAStealKeyEvent(DeviceIntPtr dev, int index, int key_code, int is_down)
{
    DGAScreenPtr pScreenPriv;
    DGAEvent event;

    if (!DGAScreenKeyRegistered)         
        return FALSE;

    if (key_code < 8 || key_code > 255)
        return FALSE;

    pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

    if (!pScreenPriv || !pScreenPriv->grabKeyboard)      
        return FALSE;

    event = (DGAEvent) {
        .header = ET_Internal,
        .type = ET_DGAEvent,
        .length = sizeof(event),
        .time = GetTimeInMillis(),
        .subtype = (is_down ? ET_KeyPress : ET_KeyRelease),
        .detail = key_code,
        .dx = 0,
        .dy = 0
    };
    mieqEnqueue(dev, (InternalEvent *) &event);

    return TRUE;
}
