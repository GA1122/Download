CursorFreeHideCount(void *data, XID id)
{
    CursorHideCountPtr pChc = (CursorHideCountPtr) data;
    ScreenPtr pScreen = pChc->pScreen;
    DeviceIntPtr dev;

    deleteCursorHideCount(pChc, pChc->pScreen);
    for (dev = inputInfo.devices; dev; dev = dev->next) {
        if (IsMaster(dev) && IsPointerDevice(dev))
            CursorDisplayCursor(dev, pScreen, CursorCurrent[dev->id]);
    }

    return 1;
}
