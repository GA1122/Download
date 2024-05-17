void RenderWidget::ConfigureAsExternalPopupMenu(const WebPopupMenuInfo& info) {
  popup_params_.reset(new ViewHostMsg_ShowPopup_Params);
  popup_params_->item_height = info.itemHeight;
  popup_params_->item_font_size = info.itemFontSize;
  popup_params_->selected_item = info.selectedIndex;
  for (size_t i = 0; i < info.items.size(); ++i)
    popup_params_->popup_items.push_back(WebMenuItem(info.items[i]));
  popup_params_->right_aligned = info.rightAligned;
}
