void AutofillPopupItemView::GetAccessibleNodeData(ui::AXNodeData* node_data) {
  AutofillPopupController* controller = popup_view_->controller();
  auto suggestion = controller->GetSuggestionAt(line_number_);
  std::vector<base::string16> text;
  text.push_back(suggestion.value);
  text.push_back(suggestion.label);
  text.push_back(suggestion.additional_label);

  base::string16 icon_description;
  if (!suggestion.icon.empty()) {
    const int id = controller->layout_model().GetIconAccessibleNameResourceId(
        suggestion.icon);
    if (id > 0)
      text.push_back(l10n_util::GetStringUTF16(id));
  }
  node_data->SetName(base::JoinString(text, base::ASCIIToUTF16(" ")));

  node_data->role = ax::mojom::Role::kMenuItem;
  node_data->AddBoolAttribute(ax::mojom::BoolAttribute::kSelected,
                              is_selected_);

  int set_size = 0;
  int pos_in_set = line_number_ + 1;
  for (int i = 0; i < controller->GetLineCount(); ++i) {
    if (controller->GetSuggestionAt(i).frontend_id ==
        autofill::POPUP_ITEM_ID_SEPARATOR) {
      if (i < line_number_)
        --pos_in_set;
    } else {
      ++set_size;
    }
  }
  node_data->AddIntAttribute(ax::mojom::IntAttribute::kSetSize, set_size);
  node_data->AddIntAttribute(ax::mojom::IntAttribute::kPosInSet, pos_in_set);
}
