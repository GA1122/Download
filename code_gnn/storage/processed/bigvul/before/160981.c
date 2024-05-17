PopupMenu* ChromeClientImpl::OpenPopupMenu(LocalFrame& frame,
                                           HTMLSelectElement& select) {
  if (frame.GetDocument()->GetSettings()->GetPagePopupsSuppressed())
    return nullptr;

  NotifyPopupOpeningObservers();
  if (WebViewImpl::UseExternalPopupMenus())
    return new ExternalPopupMenu(frame, select, *web_view_);

  DCHECK(RuntimeEnabledFeatures::PagePopupEnabled());
  return InternalPopupMenu::Create(this, select);
}
