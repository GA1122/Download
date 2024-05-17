WindowOpenDisposition BrowserView::GetDispositionForPopupBounds(
    const gfx::Rect& bounds) {
#if defined(OS_WIN)
#if defined(USE_AURA)
  return NEW_POPUP;
#else
  return base::win::IsMetroProcess() ? NEW_BACKGROUND_TAB : NEW_POPUP;
#endif
#else
  return NEW_POPUP;
#endif
}
