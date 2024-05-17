ActivateKeyboardGrab(DeviceIntPtr keybd, GrabPtr grab, TimeStamp time,
                     Bool passive)
{
    GrabInfoPtr grabinfo = &keybd->deviceGrab;
    GrabPtr oldgrab = grabinfo->grab;
    WindowPtr oldWin;

     
    if (grab->grabtype == XI2 && keybd->enabled &&
        !(passive & ImplicitGrabMask) && !IsMaster(keybd))
        DetachFromMaster(keybd);

    if (!keybd->enabled)
        oldWin = NULL;
    else if (grabinfo->grab)
        oldWin = grabinfo->grab->window;
    else if (keybd->focus)
        oldWin = keybd->focus->win;
    else
        oldWin = keybd->spriteInfo->sprite->win;
    if (oldWin == FollowKeyboardWin)
        oldWin = keybd->focus->win;
    if (keybd->valuator)
        keybd->valuator->motionHintWindow = NullWindow;
    if (oldWin)
        DoFocusEvents(keybd, oldWin, grab->window, NotifyGrab);
    if (syncEvents.playingEvents)
        grabinfo->grabTime = syncEvents.time;
    else
        grabinfo->grabTime = time;
    grabinfo->grab = AllocGrab(grab);
    grabinfo->fromPassiveGrab = passive;
    grabinfo->implicitGrab = passive & ImplicitGrabMask;
    CheckGrabForSyncs(keybd, (Bool) grab->keyboardMode,
                      (Bool) grab->pointerMode);
    if (oldgrab)
        FreeGrab(oldgrab);
}
