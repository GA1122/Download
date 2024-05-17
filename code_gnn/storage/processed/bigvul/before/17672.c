XFixesSelectCursorInput(ClientPtr pClient, WindowPtr pWindow, CARD32 eventMask)
{
    CursorEventPtr *prev, e;
    void *val;
    int rc;

    for (prev = &cursorEvents; (e = *prev); prev = &e->next) {
        if (e->pClient == pClient && e->pWindow == pWindow) {
            break;
        }
    }
    if (!eventMask) {
        if (e) {
            FreeResource(e->clientResource, 0);
        }
        return Success;
    }
    if (!e) {
        e = (CursorEventPtr) malloc(sizeof(CursorEventRec));
        if (!e)
            return BadAlloc;

        e->next = 0;
        e->pClient = pClient;
        e->pWindow = pWindow;
        e->clientResource = FakeClientID(pClient->index);

         
        rc = dixLookupResourceByType(&val, pWindow->drawable.id,
                                     CursorWindowType, serverClient,
                                     DixGetAttrAccess);
        if (rc != Success)
            if (!AddResource(pWindow->drawable.id, CursorWindowType,
                             (void *) pWindow)) {
                free(e);
                return BadAlloc;
            }

        if (!AddResource(e->clientResource, CursorClientType, (void *) e))
            return BadAlloc;

        *prev = e;
    }
    e->eventMask = eventMask;
    return Success;
}
