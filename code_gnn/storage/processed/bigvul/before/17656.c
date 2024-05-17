ProcXFixesSelectCursorInput(ClientPtr client)
{
    REQUEST(xXFixesSelectCursorInputReq);
    WindowPtr pWin;
    int rc;

    REQUEST_SIZE_MATCH(xXFixesSelectCursorInputReq);
    rc = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;
    if (stuff->eventMask & ~CursorAllEvents) {
        client->errorValue = stuff->eventMask;
        return BadValue;
    }
    return XFixesSelectCursorInput(client, pWin, stuff->eventMask);
}
