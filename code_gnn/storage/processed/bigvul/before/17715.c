DGAProcessKeyboardEvent(ScreenPtr pScreen, DGAEvent * event, DeviceIntPtr keybd)
{
    KeyClassPtr keyc = keybd->key;
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(pScreen);
    DeviceIntPtr pointer = GetMaster(keybd, POINTER_OR_FLOAT);
    DeviceEvent ev = {
        .header = ET_Internal,
        .length = sizeof(ev),
        .detail.key = event->detail,
        .type = event->subtype,
        .root_x = 0,
        .root_y = 0,
        .corestate = XkbStateFieldFromRec(&keyc->xkbInfo->state)
    };
    ev.corestate |= pointer->button->state;

    UpdateDeviceState(keybd, &ev);

    if (!IsMaster(keybd))
        return;

     
    if (pScreenPriv->client) {
        dgaEvent de = {
            .u.event.time = event->time,
            .u.event.dx = event->dx,
            .u.event.dy = event->dy,
            .u.event.screen = pScreen->myNum,
            .u.event.state = ev.corestate
        };
        de.u.u.type = DGAEventBase + GetCoreType(ev.type);
        de.u.u.detail = event->detail;

         
        TryClientEvents(pScreenPriv->client, keybd, (xEvent *) &de, 1,
                        filters[ev.type], pScreenPriv->input, 0);
    }
    else {
         
        if (keybd->deviceGrab.grab && !keybd->deviceGrab.fromPassiveGrab) {
            ev.detail.key = event->detail;
            ev.time = event->time;
            ev.root_x = event->dx;
            ev.root_y = event->dy;
            ev.corestate = event->state;
            ev.deviceid = keybd->id;
            DeliverGrabbedEvent((InternalEvent *) &ev, keybd, FALSE);
        }
    }
}
