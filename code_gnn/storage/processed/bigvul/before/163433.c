void OmniboxViewViews::GetAccessibleNodeData(ui::AXNodeData* node_data) {
  node_data->role = ui::AX_ROLE_TEXT_FIELD;
  node_data->SetName(l10n_util::GetStringUTF8(IDS_ACCNAME_LOCATION));
  node_data->SetValue(GetText());
  node_data->html_attributes.push_back(std::make_pair("type", "url"));

  base::string16::size_type entry_start;
  base::string16::size_type entry_end;
  if (saved_selection_for_focus_change_.IsValid()) {
    entry_start = saved_selection_for_focus_change_.start();
    entry_end = saved_selection_for_focus_change_.end();
  } else {
    GetSelectionBounds(&entry_start, &entry_end);
  }
  node_data->AddIntAttribute(ui::AX_ATTR_TEXT_SEL_START, entry_start);
  node_data->AddIntAttribute(ui::AX_ATTR_TEXT_SEL_END, entry_end);

  if (popup_window_mode_) {
    node_data->AddIntAttribute(ui::AX_ATTR_RESTRICTION,
                               ui::AX_RESTRICTION_READ_ONLY);
  } else {
    node_data->AddState(ui::AX_STATE_EDITABLE);
  }
}
