void RenderFrameImpl::OnSelectPopupMenuItems(
    bool canceled,
    const std::vector<int>& selected_indices) {
  if (!external_popup_menu_)
    return;

  blink::WebScopedUserGesture gesture(frame_);
  std::unique_ptr<ExternalPopupMenu> popup;
  popup.swap(external_popup_menu_);
  popup->DidSelectItems(canceled, selected_indices);
}
