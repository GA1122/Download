ProcXFixesShowCursor(ClientPtr client)
{
    WindowPtr pWin;
    CursorHideCountPtr pChc;
    int rc;

    REQUEST(xXFixesShowCursorReq);

    REQUEST_SIZE_MATCH(xXFixesShowCursorReq);

    rc = dixLookupResourceByType((void **) &pWin, stuff->window, RT_WINDOW,
                                 client, DixGetAttrAccess);
    if (rc != Success) {
        client->errorValue = stuff->window;
        return rc;
    }

     
    pChc = findCursorHideCount(client, pWin->drawable.pScreen);
    if (pChc == NULL) {
        return BadMatch;
    }

    rc = XaceHook(XACE_SCREEN_ACCESS, client, pWin->drawable.pScreen,
                  DixShowAccess);
    if (rc != Success)
        return rc;

    pChc->hideCount--;
    if (pChc->hideCount <= 0) {
        FreeResource(pChc->resource, 0);
    }

    return Success;
}
