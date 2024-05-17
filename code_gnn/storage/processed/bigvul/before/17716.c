DGAProcessPointerEvent(ScreenPtr pScreen, DGAEvent * event, DeviceIntPtr mouse)
{
    ButtonClassPtr butc = mouse->button;
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(pScreen);
    DeviceIntPtr master = GetMaster(mouse, MASTER_KEYBOARD);
    DeviceEvent ev = {
        .header = ET_Internal,
        .length = sizeof(ev),
        .detail.key = event->detail,
        .type = event->subtype,
        .corestate = butc ? butc->state : 0
    };

    if (master && master->key)
        ev.corestate |= XkbStateFieldFromRec(&master->key->xkbInfo->state);

    UpdateDeviceState(mouse, &ev);

    if (!IsMaster(mouse))
        return;

     
    if (pScreenPriv->client) {
        int coreEquiv = GetCoreType(ev.type);
        dgaEvent de = {
            .u.event.time = event->time,
            .u.event.dx = event->dx,
            .u.event.dy = event->dy,
            .u.event.screen = pScreen->myNum,
            .u.event.state = ev.corestate
        };
        de.u.u.type = DGAEventBase + coreEquiv;
        de.u.u.detail = event->detail;

         
        TryClientEvents(pScreenPriv->client, mouse, (xEvent *) &de, 1,
                        filters[coreEquiv], pScreenPriv->input, 0);
    }
    else {
         
        if (mouse->deviceGrab.grab && !mouse->deviceGrab.fromPassiveGrab) {
            ev.detail.button = event->detail;
            ev.time = event->time;
            ev.root_x = event->dx;
            ev.root_y = event->dy;
            ev.corestate = event->state;
             
            SetBit(ev.valuators.mask, 0);
            SetBit(ev.valuators.mask, 1);
            DeliverGrabbedEvent((InternalEvent *) &ev, mouse, FALSE);
        }
    }
}
