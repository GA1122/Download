DeliverEventsToWindow(DeviceIntPtr pDev, WindowPtr pWin, xEvent
                      *pEvents, int count, Mask filter, GrabPtr grab)
{
    int deliveries = 0, nondeliveries = 0;
    ClientPtr client = NullClient;
    Mask deliveryMask = 0;       
    int type = pEvents->u.u.type;

     
    if ((filter == CantBeFiltered) || core_get_type(pEvents) != 0) {
        enum EventDeliveryState rc;

        rc = DeliverToWindowOwner(pDev, pWin, pEvents, count, filter, grab);

        switch (rc) {
        case EVENT_SKIP:
            return 0;
        case EVENT_REJECTED:
            nondeliveries--;
            break;
        case EVENT_DELIVERED:
             
            deliveries++;
            client = wClient(pWin);
            deliveryMask = pWin->eventMask;
            break;
        case EVENT_NOT_DELIVERED:
            break;
        }
    }

     
    if (filter != CantBeFiltered) {
        enum EventDeliveryState rc;

        rc = DeliverEventToWindowMask(pDev, pWin, pEvents, count, filter,
                                      grab, &client, &deliveryMask);

        switch (rc) {
        case EVENT_SKIP:
            return 0;
        case EVENT_REJECTED:
            nondeliveries--;
            break;
        case EVENT_DELIVERED:
            deliveries++;
            break;
        case EVENT_NOT_DELIVERED:
            break;
        }
    }

    if (deliveries) {
         
        if (!grab &&
            ActivateImplicitGrab(pDev, client, pWin, pEvents, deliveryMask))
              ;
        else if (type == MotionNotify)
            pDev->valuator->motionHintWindow = pWin;
        else if (type == DeviceMotionNotify || type == DeviceButtonPress)
            CheckDeviceGrabAndHintWindow(pWin, type,
                                         (deviceKeyButtonPointer *) pEvents,
                                         grab, client, deliveryMask);
        return deliveries;
    }
    return nondeliveries;
}
