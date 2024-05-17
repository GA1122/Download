DGAHandleEvent(int screen_num, InternalEvent *ev, DeviceIntPtr device)
{
    DGAEvent *event = &ev->dga_event;
    ScreenPtr pScreen = screenInfo.screens[screen_num];
    DGAScreenPtr pScreenPriv;

     
    if (!DGAScreenKeyRegistered || noXFree86DGAExtension)
	return;
    pScreenPriv = DGA_GET_SCREEN_PRIV(pScreen);

     
    if (!pScreenPriv)
        return;

    switch (event->subtype) {
    case KeyPress:
    case KeyRelease:
        DGAProcessKeyboardEvent(pScreen, event, device);
        break;
    case MotionNotify:
    case ButtonPress:
    case ButtonRelease:
        DGAProcessPointerEvent(pScreen, event, device);
        break;
    default:
        break;
    }
}
