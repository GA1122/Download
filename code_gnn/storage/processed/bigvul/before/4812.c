DeliverEventToInputClients(DeviceIntPtr dev, InputClients * inputclients,
                           WindowPtr win, xEvent *events,
                           int count, Mask filter, GrabPtr grab,
                           ClientPtr *client_return, Mask *mask_return)
{
    int attempt;
    enum EventDeliveryState rc = EVENT_NOT_DELIVERED;
    Bool have_device_button_grab_class_client = FALSE;

    for (; inputclients; inputclients = inputclients->next) {
        Mask mask;
        ClientPtr client = rClient(inputclients);

        if (IsInterferingGrab(client, dev, events))
            continue;

        if (IsWrongPointerBarrierClient(client, dev, events))
            continue;

        mask = GetEventMask(dev, events, inputclients);

        if (XaceHook(XACE_RECEIVE_ACCESS, client, win, events, count))
              ;
        else if ((attempt = TryClientEvents(client, dev,
                                            events, count,
                                            mask, filter, grab))) {
            if (attempt > 0) {
                 
                if (!have_device_button_grab_class_client) {
                    rc = EVENT_DELIVERED;
                    *client_return = client;
                    *mask_return = mask;
                     
                    if (mask & DeviceButtonGrabMask)
                        have_device_button_grab_class_client = TRUE;
                }
            } else if (rc == EVENT_NOT_DELIVERED)
                rc = EVENT_REJECTED;
        }
    }

    return rc;
}
