void AutofillPopupWarningView::GetAccessibleNodeData(
    ui::AXNodeData* node_data) {
  AutofillPopupController* controller = popup_view_->controller();
  if (!controller)
    return;

  node_data->SetName(controller->GetSuggestionAt(line_number_).value);
  node_data->role = ax::mojom::Role::kStaticText;
}
