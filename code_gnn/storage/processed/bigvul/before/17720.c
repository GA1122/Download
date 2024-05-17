DGASetInputMode(int index, Bool keyboard, Bool mouse)
{
    ScreenPtr pScreen = screenInfo.screens[index];
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(pScreen);

    if (pScreenPriv) {
        pScreenPriv->grabMouse = mouse;
        pScreenPriv->grabKeyboard = keyboard;

        if (!mieq_installed) {
            mieqSetHandler(ET_DGAEvent, DGAHandleEvent);
            mieq_installed = TRUE;
        }
    }
}
