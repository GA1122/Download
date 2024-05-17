void BrowserView::DisableInactiveFrame() {
#if defined(OS_WIN) && !defined(USE_AURA)
  frame_->DisableInactiveRendering();
#endif   
}
