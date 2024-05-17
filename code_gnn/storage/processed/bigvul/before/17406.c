ProcScreenSaverSelectInput(ClientPtr client)
{
    REQUEST(xScreenSaverSelectInputReq);
    DrawablePtr pDraw;
    int rc;

    REQUEST_SIZE_MATCH(xScreenSaverSelectInputReq);
    rc = dixLookupDrawable(&pDraw, stuff->drawable, client, 0,
                           DixGetAttrAccess);
    if (rc != Success)
        return rc;

    rc = XaceHook(XACE_SCREENSAVER_ACCESS, client, pDraw->pScreen,
                  DixSetAttrAccess);
    if (rc != Success)
        return rc;

    if (!setEventMask(pDraw->pScreen, client, stuff->eventMask))
        return BadAlloc;
    return Success;
}
