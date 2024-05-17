ReplaceCursorLookup(void *value, XID id, void *closure)
{
    ReplaceCursorLookupPtr rcl = (ReplaceCursorLookupPtr) closure;
    WindowPtr pWin;
    GrabPtr pGrab;
    CursorPtr pCursor = 0, *pCursorRef = 0;
    XID cursor = 0;

    switch (rcl->type) {
    case RT_WINDOW:
        pWin = (WindowPtr) value;
        if (pWin->optional) {
            pCursorRef = &pWin->optional->cursor;
            pCursor = *pCursorRef;
        }
        break;
    case RT_PASSIVEGRAB:
        pGrab = (GrabPtr) value;
        pCursorRef = &pGrab->cursor;
        pCursor = *pCursorRef;
        break;
    case RT_CURSOR:
        pCursorRef = 0;
        pCursor = (CursorPtr) value;
        cursor = id;
        break;
    }
    if (pCursor && pCursor != rcl->pNew) {
        if ((*rcl->testCursor) (pCursor, rcl->closure)) {
            CursorPtr curs = RefCursor(rcl->pNew);
             
            if (pCursorRef)
                *pCursorRef = curs;
            else
                ChangeResourceValue(id, RT_CURSOR, curs);
            FreeCursor(pCursor, cursor);
        }
    }
    return FALSE;                
}
