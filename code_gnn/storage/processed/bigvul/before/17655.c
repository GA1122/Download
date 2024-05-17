ProcXFixesHideCursor(ClientPtr client)
{
    WindowPtr pWin;
    CursorHideCountPtr pChc;

    REQUEST(xXFixesHideCursorReq);
    int ret;

    REQUEST_SIZE_MATCH(xXFixesHideCursorReq);

    ret = dixLookupResourceByType((void **) &pWin, stuff->window, RT_WINDOW,
                                  client, DixGetAttrAccess);
    if (ret != Success) {
        client->errorValue = stuff->window;
        return ret;
    }

     

    pChc = findCursorHideCount(client, pWin->drawable.pScreen);
    if (pChc != NULL) {
        pChc->hideCount++;
        return Success;
    }

     
    ret = XaceHook(XACE_SCREEN_ACCESS, client, pWin->drawable.pScreen,
                   DixHideAccess);
    if (ret != Success)
        return ret;

    ret = createCursorHideCount(client, pWin->drawable.pScreen);

    if (ret == Success) {
        DeviceIntPtr dev;

        for (dev = inputInfo.devices; dev; dev = dev->next) {
            if (IsMaster(dev) && IsPointerDevice(dev))
                CursorDisplayCursor(dev, pWin->drawable.pScreen,
                                    CursorCurrent[dev->id]);
        }
    }

    return ret;
}
