ProcRecolorCursor(ClientPtr client)
{
    CursorPtr pCursor;
    int rc, nscr;
    ScreenPtr pscr;
    Bool displayed;
    SpritePtr pSprite = PickPointer(client)->spriteInfo->sprite;

    REQUEST(xRecolorCursorReq);

    REQUEST_SIZE_MATCH(xRecolorCursorReq);
    rc = dixLookupResourceByType((void **) &pCursor, stuff->cursor, RT_CURSOR,
                                 client, DixWriteAccess);
    if (rc != Success) {
        client->errorValue = stuff->cursor;
        return rc;
    }

    pCursor->foreRed = stuff->foreRed;
    pCursor->foreGreen = stuff->foreGreen;
    pCursor->foreBlue = stuff->foreBlue;

    pCursor->backRed = stuff->backRed;
    pCursor->backGreen = stuff->backGreen;
    pCursor->backBlue = stuff->backBlue;

    for (nscr = 0; nscr < screenInfo.numScreens; nscr++) {
        pscr = screenInfo.screens[nscr];
#ifdef PANORAMIX
        if (!noPanoramiXExtension)
            displayed = (pscr == pSprite->screen);
        else
#endif
            displayed = (pscr == pSprite->hotPhys.pScreen);
        (*pscr->RecolorCursor) (PickPointer(client), pscr, pCursor,
                                (pCursor == pSprite->current) && displayed);
    }
    return Success;
}
