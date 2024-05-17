int BrowserView::GetOTRIconResourceID() const {
  int otr_resource_id = IDR_OTR_ICON;
  if (ui::GetDisplayLayout() == ui::LAYOUT_TOUCH) {
    if (IsFullscreen())
      otr_resource_id = IDR_OTR_ICON_FULLSCREEN;
#if defined(OS_WIN) && !defined(USE_AURA)
    if (base::win::IsMetroProcess())
      otr_resource_id = IDR_OTR_ICON_FULLSCREEN;
#endif
  }

  return otr_resource_id;
}
