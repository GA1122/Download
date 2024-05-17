Browser* DefaultTabHandler::CreateNewStripWithContents(
    TabContentsWrapper* detached_contents,
    const gfx::Rect& window_bounds,
    const DockInfo& dock_info,
    bool maximize) {
  return delegate_->AsBrowser()->CreateNewStripWithContents(detached_contents,
                                                            window_bounds,
                                                            dock_info,
                                                            maximize);
}
