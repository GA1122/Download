CursorDisplayCursor(DeviceIntPtr pDev, ScreenPtr pScreen, CursorPtr pCursor)
{
    CursorScreenPtr cs = GetCursorScreen(pScreen);
    Bool ret;
    DisplayCursorProcPtr backupProc;

    Unwrap(cs, pScreen, DisplayCursor, backupProc);

    CursorVisible = CursorVisible && EnableCursor;

    if (cs->pCursorHideCounts != NULL || !CursorVisible) {
        ret = (*pScreen->DisplayCursor) (pDev, pScreen, NullCursor);
    }
    else {
        ret = (*pScreen->DisplayCursor) (pDev, pScreen, pCursor);
    }

    if (pCursor != CursorCurrent[pDev->id]) {
        CursorEventPtr e;

        UpdateCurrentTimeIf();
        CursorCurrent[pDev->id] = pCursor;
        for (e = cursorEvents; e; e = e->next) {
            if ((e->eventMask & XFixesDisplayCursorNotifyMask)) {
                xXFixesCursorNotifyEvent ev = {
                    .type = XFixesEventBase + XFixesCursorNotify,
                    .subtype = XFixesDisplayCursorNotify,
                    .window = e->pWindow->drawable.id,
                    .cursorSerial = pCursor ? pCursor->serialNumber : 0,
                    .timestamp = currentTime.milliseconds,
                    .name = pCursor ? pCursor->name : None
                };
                WriteEventsToClient(e->pClient, 1, (xEvent *) &ev);
            }
        }
    }
    Wrap(cs, pScreen, DisplayCursor, backupProc);

    return ret;
}
