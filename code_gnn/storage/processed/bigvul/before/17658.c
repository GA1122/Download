ReplaceCursor(CursorPtr pCursor, TestCursorFunc testCursor, void *closure)
{
    int clientIndex;
    int resIndex;
    ReplaceCursorLookupRec rcl;

     
    rcl.testCursor = testCursor;
    rcl.pNew = pCursor;
    rcl.closure = closure;

     
    for (clientIndex = 0; clientIndex < currentMaxClients; clientIndex++) {
        if (!clients[clientIndex])
            continue;
        for (resIndex = 0; resIndex < NUM_CURSOR_RESTYPES; resIndex++) {
            rcl.type = CursorRestypes[resIndex];
             
            LookupClientResourceComplex(clients[clientIndex],
                                        rcl.type,
                                        ReplaceCursorLookup, (void *) &rcl);
        }
    }
     
    WindowHasNewCursor(screenInfo.screens[0]->root);
}
