findCursorHideCount(ClientPtr pClient, ScreenPtr pScreen)
{
    CursorScreenPtr cs = GetCursorScreen(pScreen);
    CursorHideCountPtr pChc;

    for (pChc = cs->pCursorHideCounts; pChc != NULL; pChc = pChc->pNext) {
        if (pChc->pClient == pClient) {
            return pChc;
        }
    }

    return NULL;
}
