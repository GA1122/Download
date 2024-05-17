CoreFocusEvent(DeviceIntPtr dev, int type, int mode, int detail, WindowPtr pWin)
{
    xEvent event = {
        .u.u.type = type,
        .u.u.detail = detail
    };
    event.u.focus.mode = mode;
    event.u.focus.window = pWin->drawable.id;

    DeliverEventsToWindow(dev, pWin, &event, 1,
                          GetEventFilter(dev, &event), NullGrab);
    if ((type == FocusIn) &&
        ((pWin->eventMask | wOtherEventMasks(pWin)) & KeymapStateMask)) {
        xKeymapEvent ke = {
            .type = KeymapNotify
        };
        ClientPtr client = wClient(pWin);
        int rc;

        rc = XaceHook(XACE_DEVICE_ACCESS, client, dev, DixReadAccess);
        if (rc == Success)
            memcpy((char *) &ke.map[0], (char *) &dev->key->down[1], 31);

        DeliverEventsToWindow(dev, pWin, (xEvent *) &ke, 1,
                              KeymapStateMask, NullGrab);
    }
}
