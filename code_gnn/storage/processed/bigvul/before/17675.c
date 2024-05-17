deleteCursorHideCountsForScreen(ScreenPtr pScreen)
{
    CursorScreenPtr cs = GetCursorScreen(pScreen);
    CursorHideCountPtr pChc, pTmp;

    pChc = cs->pCursorHideCounts;
    while (pChc != NULL) {
        pTmp = pChc->pNext;
        FreeResource(pChc->resource, 0);
        pChc = pTmp;
    }
    cs->pCursorHideCounts = NULL;
}
