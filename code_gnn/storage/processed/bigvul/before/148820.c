void InterstitialPageImpl::UpdateTitle(
    RenderFrameHost* render_frame_host,
    const base::string16& title,
    base::i18n::TextDirection title_direction) {
  if (!enabled())
    return;

  RenderViewHost* render_view_host = render_frame_host->GetRenderViewHost();
  DCHECK(render_view_host == render_view_host_);
  NavigationEntry* entry = controller_->GetVisibleEntry();
  if (!entry) {
    return;
  }

  if (!new_navigation_ && !should_revert_web_contents_title_) {
    original_web_contents_title_ = entry->GetTitle();
    should_revert_web_contents_title_ = true;
  }
  web_contents_->UpdateTitleForEntry(entry, title);
}
