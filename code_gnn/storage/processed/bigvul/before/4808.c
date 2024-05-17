DeactivatePointerGrab(DeviceIntPtr mouse)
{
    GrabPtr grab = mouse->deviceGrab.grab;
    DeviceIntPtr dev;
    Bool wasPassive = mouse->deviceGrab.fromPassiveGrab;
    Bool wasImplicit = (mouse->deviceGrab.fromPassiveGrab &&
                        mouse->deviceGrab.implicitGrab);
    XID grab_resource = grab->resource;
    int i;

     
    for (i = 0; !wasPassive && mouse->touch && i < mouse->touch->num_touches; i++) {
        TouchPointInfoPtr ti = mouse->touch->touches + i;
        if (ti->active && TouchResourceIsOwner(ti, grab_resource)) {
            int mode = XIRejectTouch;
             
            if (grab->grabtype == CORE || grab->grabtype == XI ||
                    !xi2mask_isset(mouse->deviceGrab.grab->xi2mask, mouse, XI_TouchBegin)) {
                mode = XIAcceptTouch;
                 
                ti->listeners[0].state = LISTENER_HAS_END;
            }
            TouchListenerAcceptReject(mouse, ti, 0, mode);
        }
    }

    TouchRemovePointerGrab(mouse);

    mouse->valuator->motionHintWindow = NullWindow;
    mouse->deviceGrab.grab = NullGrab;
    mouse->deviceGrab.sync.state = NOT_GRABBED;
    mouse->deviceGrab.fromPassiveGrab = FALSE;

    for (dev = inputInfo.devices; dev; dev = dev->next) {
        if (dev->deviceGrab.sync.other == grab)
            dev->deviceGrab.sync.other = NullGrab;
    }
    DoEnterLeaveEvents(mouse, mouse->id, grab->window,
                       mouse->spriteInfo->sprite->win, NotifyUngrab);
    if (grab->confineTo)
        ConfineCursorToWindow(mouse, GetCurrentRootWindow(mouse), FALSE, FALSE);
    PostNewCursor(mouse);

    if (!wasImplicit && grab->grabtype == XI2)
        ReattachToOldMaster(mouse);

    ComputeFreezes();

    FreeGrab(grab);
}
