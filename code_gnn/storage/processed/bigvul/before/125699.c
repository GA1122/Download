void RenderViewHostImpl::OnUpdateTitle(
    int32 page_id,
    const string16& title,
    WebKit::WebTextDirection title_direction) {
  if (title.length() > kMaxTitleChars) {
    NOTREACHED() << "Renderer sent too many characters in title.";
    return;
  }

  delegate_->UpdateTitle(this, page_id, title,
                         WebTextDirectionToChromeTextDirection(
                             title_direction));
}
