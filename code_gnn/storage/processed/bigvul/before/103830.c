void RenderView::OnSelectPopupMenuItem(int selected_index) {
  if (external_popup_menu_ == NULL) {
    NOTREACHED();
    return;
  }
  external_popup_menu_->DidSelectItem(selected_index);
  external_popup_menu_.reset();
}
