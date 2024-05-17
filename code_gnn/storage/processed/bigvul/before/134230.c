bool BrowserRootView::CanDrop(const ui::OSExchangeData& data) {
  if (!tabstrip() || !tabstrip()->visible())
    return false;

  if (data.HasURL())
    return true;

  return GetPasteAndGoURL(data, NULL);
}
