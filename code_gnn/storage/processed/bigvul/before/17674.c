deleteCursorHideCount(CursorHideCountPtr pChcToDel, ScreenPtr pScreen)
{
    CursorScreenPtr cs = GetCursorScreen(pScreen);
    CursorHideCountPtr pChc, pNext;
    CursorHideCountPtr pChcLast = NULL;

    pChc = cs->pCursorHideCounts;
    while (pChc != NULL) {
        pNext = pChc->pNext;
        if (pChc == pChcToDel) {
            free(pChc);
            if (pChcLast == NULL) {
                cs->pCursorHideCounts = pNext;
            }
            else {
                pChcLast->pNext = pNext;
            }
            return;
        }
        pChcLast = pChc;
        pChc = pNext;
    }
}
