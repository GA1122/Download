DeleteWindowFromAnyEvents(WindowPtr pWin, Bool freeResources)
{
    WindowPtr parent;
    DeviceIntPtr mouse = inputInfo.pointer;
    DeviceIntPtr keybd = inputInfo.keyboard;
    FocusClassPtr focus;
    OtherClientsPtr oc;
    GrabPtr passive;
    GrabPtr grab;

     
    grab = mouse->deviceGrab.grab;
    if (grab && ((grab->window == pWin) || (grab->confineTo == pWin)))
        (*mouse->deviceGrab.DeactivateGrab) (mouse);

     
    grab = keybd->deviceGrab.grab;
    if (grab && (grab->window == pWin))
        (*keybd->deviceGrab.DeactivateGrab) (keybd);

     
    for (mouse = inputInfo.devices; mouse; mouse = mouse->next) {
        grab = mouse->deviceGrab.grab;
        if (grab && ((grab->window == pWin) || (grab->confineTo == pWin)))
            (*mouse->deviceGrab.DeactivateGrab) (mouse);
    }

    for (keybd = inputInfo.devices; keybd; keybd = keybd->next) {
        if (IsKeyboardDevice(keybd)) {
            focus = keybd->focus;

             

            if ((pWin == focus->win) && (pWin->parent != NullWindow)) {
                int focusEventMode = NotifyNormal;

                 

                if (keybd->deviceGrab.grab)
                    focusEventMode = NotifyWhileGrabbed;

                switch (focus->revert) {
                case RevertToNone:
                    DoFocusEvents(keybd, pWin, NoneWin, focusEventMode);
                    focus->win = NoneWin;
                    focus->traceGood = 0;
                    break;
                case RevertToParent:
                    parent = pWin;
                    do {
                        parent = parent->parent;
                        focus->traceGood--;
                    } while (!parent->realized
                     
#ifdef NOTDEF
                             || wClient(parent)->clientGone
#endif
                        );
                    if (!ActivateFocusInGrab(keybd, pWin, parent))
                        DoFocusEvents(keybd, pWin, parent, focusEventMode);
                    focus->win = parent;
                    focus->revert = RevertToNone;
                    break;
                case RevertToPointerRoot:
                    if (!ActivateFocusInGrab(keybd, pWin, PointerRootWin))
                        DoFocusEvents(keybd, pWin, PointerRootWin,
                                      focusEventMode);
                    focus->win = PointerRootWin;
                    focus->traceGood = 0;
                    break;
                }
            }
        }

        if (IsPointerDevice(keybd)) {
            if (keybd->valuator->motionHintWindow == pWin)
                keybd->valuator->motionHintWindow = NullWindow;
        }
    }

    if (freeResources) {
        if (pWin->dontPropagate)
            DontPropagateRefCnts[pWin->dontPropagate]--;
        while ((oc = wOtherClients(pWin)))
            FreeResource(oc->resource, RT_NONE);
        while ((passive = wPassiveGrabs(pWin)))
            FreeResource(passive->resource, RT_NONE);
    }

    DeleteWindowFromAnyExtEvents(pWin, freeResources);
}
