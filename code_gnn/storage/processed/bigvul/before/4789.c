ActivatePassiveGrab(DeviceIntPtr device, GrabPtr grab, InternalEvent *event,
                    InternalEvent *real_event)
{
    SpritePtr pSprite = device->spriteInfo->sprite;
    GrabInfoPtr grabinfo = &device->deviceGrab;
    xEvent *xE = NULL;
    int count;
    int rc;

     
    if (grab->grabtype == XI || grab->grabtype == CORE) {
        DeviceIntPtr gdev;

        event->device_event.corestate &= 0x1f00;

        if (grab->grabtype == CORE)
            gdev = GetMaster(device, KEYBOARD_OR_FLOAT);
        else
            gdev = grab->modifierDevice;

        if (gdev && gdev->key && gdev->key->xkbInfo)
            event->device_event.corestate |=
                gdev->key->xkbInfo->state.grab_mods & (~0x1f00);
    }

    if (grab->grabtype == CORE) {
        rc = EventToCore(event, &xE, &count);
        if (rc != Success) {
            BUG_WARN_MSG(rc != BadMatch, "[dix] %s: core conversion failed"
                         "(%d, %d).\n", device->name, event->any.type, rc);
            return FALSE;
        }
    }
    else if (grab->grabtype == XI2) {
        rc = EventToXI2(event, &xE);
        if (rc != Success) {
            if (rc != BadMatch)
                BUG_WARN_MSG(rc != BadMatch, "[dix] %s: XI2 conversion failed"
                             "(%d, %d).\n", device->name, event->any.type, rc);
            return FALSE;
        }
        count = 1;
    }
    else {
        rc = EventToXI(event, &xE, &count);
        if (rc != Success) {
            if (rc != BadMatch)
                BUG_WARN_MSG(rc != BadMatch, "[dix] %s: XI conversion failed"
                             "(%d, %d).\n", device->name, event->any.type, rc);
            return FALSE;
        }
    }

    (*grabinfo->ActivateGrab) (device, grab,
                               ClientTimeToServerTime(event->any.time), TRUE);

    if (xE) {
        FixUpEventFromWindow(pSprite, xE, grab->window, None, TRUE);

         
        TryClientEvents(rClient(grab), device, xE, count,
                        GetEventFilter(device, xE),
                        GetEventFilter(device, xE), grab);
    }

    if (grabinfo->sync.state == FROZEN_NO_EVENT)
        grabinfo->sync.state = FROZEN_WITH_EVENT;
    *grabinfo->sync.event = real_event->device_event;

    free(xE);
    return TRUE;
}
