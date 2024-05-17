ProcScreenSaverQueryInfo(ClientPtr client)
{
    REQUEST(xScreenSaverQueryInfoReq);
    xScreenSaverQueryInfoReply rep;
    int rc;
    ScreenSaverStuffPtr pSaver;
    DrawablePtr pDraw;
    CARD32 lastInput;
    ScreenSaverScreenPrivatePtr pPriv;

    REQUEST_SIZE_MATCH(xScreenSaverQueryInfoReq);
    rc = dixLookupDrawable(&pDraw, stuff->drawable, client, 0,
                           DixGetAttrAccess);
    if (rc != Success)
        return rc;
    rc = XaceHook(XACE_SCREENSAVER_ACCESS, client, pDraw->pScreen,
                  DixGetAttrAccess);
    if (rc != Success)
        return rc;

    pSaver = &pDraw->pScreen->screensaver;
    pPriv = GetScreenPrivate(pDraw->pScreen);

    UpdateCurrentTime();
    lastInput = GetTimeInMillis() - LastEventTime(XIAllDevices).milliseconds;

    rep = (xScreenSaverQueryInfoReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .window = pSaver->wid
    };
    if (screenIsSaved != SCREEN_SAVER_OFF) {
        rep.state = ScreenSaverOn;
        if (ScreenSaverTime)
            rep.tilOrSince = lastInput - ScreenSaverTime;
        else
            rep.tilOrSince = 0;
    }
    else {
        if (ScreenSaverTime) {
            rep.state = ScreenSaverOff;
            if (ScreenSaverTime < lastInput)
                rep.tilOrSince = 0;
            else
                rep.tilOrSince = ScreenSaverTime - lastInput;
        }
        else {
            rep.state = ScreenSaverDisabled;
            rep.tilOrSince = 0;
        }
    }
    rep.idle = lastInput;
    rep.eventMask = getEventMask(pDraw->pScreen, client);
    if (pPriv && pPriv->attr)
        rep.kind = ScreenSaverExternal;
    else if (ScreenSaverBlanking != DontPreferBlanking)
        rep.kind = ScreenSaverBlanked;
    else
        rep.kind = ScreenSaverInternal;
    if (client->swapped) {
        swaps(&rep.sequenceNumber);
        swapl(&rep.length);
        swapl(&rep.window);
        swapl(&rep.tilOrSince);
        swapl(&rep.idle);
        swapl(&rep.eventMask);
    }
    WriteToClient(client, sizeof(xScreenSaverQueryInfoReply), &rep);
    return Success;
}
