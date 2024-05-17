CursorCloseScreen(ScreenPtr pScreen)
{
    CursorScreenPtr cs = GetCursorScreen(pScreen);
    Bool ret;
    _X_UNUSED CloseScreenProcPtr close_proc;
    _X_UNUSED DisplayCursorProcPtr display_proc;

    Unwrap(cs, pScreen, CloseScreen, close_proc);
    Unwrap(cs, pScreen, DisplayCursor, display_proc);
    deleteCursorHideCountsForScreen(pScreen);
    ret = (*pScreen->CloseScreen) (pScreen);
    free(cs);
    return ret;
}
