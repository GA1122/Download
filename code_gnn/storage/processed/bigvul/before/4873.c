ProcChangeActivePointerGrab(ClientPtr client)
{
    DeviceIntPtr device;
    GrabPtr grab;
    CursorPtr newCursor, oldCursor;

    REQUEST(xChangeActivePointerGrabReq);
    TimeStamp time;

    REQUEST_SIZE_MATCH(xChangeActivePointerGrabReq);
    if (stuff->eventMask & ~PointerGrabMask) {
        client->errorValue = stuff->eventMask;
        return BadValue;
    }
    if (stuff->cursor == None)
        newCursor = NullCursor;
    else {
        int rc = dixLookupResourceByType((void **) &newCursor, stuff->cursor,
                                         RT_CURSOR, client, DixUseAccess);

        if (rc != Success) {
            client->errorValue = stuff->cursor;
            return rc;
        }
    }

    device = PickPointer(client);
    grab = device->deviceGrab.grab;

    if (!grab)
        return Success;
    if (!SameClient(grab, client))
        return Success;
    UpdateCurrentTime();
    time = ClientTimeToServerTime(stuff->time);
    if ((CompareTimeStamps(time, currentTime) == LATER) ||
        (CompareTimeStamps(time, device->deviceGrab.grabTime) == EARLIER))
        return Success;
    oldCursor = grab->cursor;
    grab->cursor = RefCursor(newCursor);
    PostNewCursor(device);
    if (oldCursor)
        FreeCursor(oldCursor, (Cursor) 0);
    grab->eventMask = stuff->eventMask;
    return Success;
}
