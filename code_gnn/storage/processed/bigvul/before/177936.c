 ProcSendEvent(ClientPtr client)
 {
     WindowPtr pWin;
     WindowPtr effectiveFocus = NullWindow;       
     DeviceIntPtr dev = PickPointer(client);
     DeviceIntPtr keybd = GetMaster(dev, MASTER_KEYBOARD);
     SpritePtr pSprite = dev->spriteInfo->sprite;
 
     REQUEST(xSendEventReq);
 
     REQUEST_SIZE_MATCH(xSendEventReq);
 
      
 
     stuff->event.u.u.type &= ~(SEND_EVENT_BIT);
 
      
 
     if (!((stuff->event.u.u.type > X_Reply &&
            stuff->event.u.u.type < LASTEvent) ||
           (stuff->event.u.u.type >= EXTENSION_EVENT_BASE &&
            stuff->event.u.u.type < (unsigned) lastEvent))) {
          client->errorValue = stuff->event.u.u.type;
          return BadValue;
      }
//      
//     if (stuff->event.u.u.type == GenericEvent) {
//         client->errorValue = stuff->event.u.u.type;
//         return BadValue;
//     }
      if (stuff->event.u.u.type == ClientMessage &&
          stuff->event.u.u.detail != 8 &&
          stuff->event.u.u.detail != 16 && stuff->event.u.u.detail != 32) {
     }
 
     if (stuff->destination == PointerWindow)
         pWin = pSprite->win;
     else if (stuff->destination == InputFocus) {
         WindowPtr inputFocus = (keybd) ? keybd->focus->win : NoneWin;
 
         if (inputFocus == NoneWin)
             return Success;
 
          
         if (inputFocus == PointerRootWin)
             inputFocus = GetCurrentRootWindow(dev);
 
         if (IsParent(inputFocus, pSprite->win)) {
             effectiveFocus = inputFocus;
             pWin = pSprite->win;
         }
         else
             effectiveFocus = pWin = inputFocus;
     }
     else
         dixLookupWindow(&pWin, stuff->destination, client, DixSendAccess);
 
     if (!pWin)
         return BadWindow;
     if ((stuff->propagate != xFalse) && (stuff->propagate != xTrue)) {
         client->errorValue = stuff->propagate;
         return BadValue;
     }
     stuff->event.u.u.type |= SEND_EVENT_BIT;
     if (stuff->propagate) {
         for (; pWin; pWin = pWin->parent) {
             if (XaceHook(XACE_SEND_ACCESS, client, NULL, pWin,
                          &stuff->event, 1))
                 return Success;
             if (DeliverEventsToWindow(dev, pWin,
                                       &stuff->event, 1, stuff->eventMask,
                                       NullGrab))
                 return Success;
             if (pWin == effectiveFocus)
                 return Success;
             stuff->eventMask &= ~wDontPropagateMask(pWin);
             if (!stuff->eventMask)
                 break;
         }
     }
     else if (!XaceHook(XACE_SEND_ACCESS, client, NULL, pWin, &stuff->event, 1))
         DeliverEventsToWindow(dev, pWin, &stuff->event,
                               1, stuff->eventMask, NullGrab);
     return Success;
 }