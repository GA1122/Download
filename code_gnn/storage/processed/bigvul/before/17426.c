setEventMask(ScreenPtr pScreen, ClientPtr client, unsigned long mask)
{
    SetupScreen(pScreen);
    ScreenSaverEventPtr pEv, *pPrev;

    if (getEventMask(pScreen, client) == mask)
        return TRUE;
    if (!pPriv) {
        pPriv = MakeScreenPrivate(pScreen);
        if (!pPriv)
            return FALSE;
    }
    for (pPrev = &pPriv->events; (pEv = *pPrev) != 0; pPrev = &pEv->next)
        if (pEv->client == client)
            break;
    if (mask == 0) {
        FreeResource(pEv->resource, SaverEventType);
        *pPrev = pEv->next;
        free(pEv);
        CheckScreenPrivate(pScreen);
    }
    else {
        if (!pEv) {
            pEv = New(ScreenSaverEventRec);
            if (!pEv) {
                CheckScreenPrivate(pScreen);
                return FALSE;
            }
            *pPrev = pEv;
            pEv->next = NULL;
            pEv->client = client;
            pEv->screen = pScreen;
            pEv->resource = FakeClientID(client->index);
            if (!AddResource(pEv->resource, SaverEventType, (void *) pEv))
                return FALSE;
        }
        pEv->mask = mask;
    }
    return TRUE;
}
