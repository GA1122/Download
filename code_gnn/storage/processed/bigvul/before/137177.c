void Textfield::GetAccessibleNodeData(ui::AXNodeData* node_data) {
  node_data->role = ax::mojom::Role::kTextField;
  if (label_ax_id_) {
    node_data->AddIntListAttribute(ax::mojom::IntListAttribute::kLabelledbyIds,
                                   {label_ax_id_});
  }

  node_data->SetName(accessible_name_);
  node_data->AddState(ax::mojom::State::kEditable);
  if (enabled()) {
    node_data->SetDefaultActionVerb(ax::mojom::DefaultActionVerb::kActivate);
    if (read_only())
      node_data->SetRestriction(ax::mojom::Restriction::kReadOnly);
  }
  if (text_input_type_ == ui::TEXT_INPUT_TYPE_PASSWORD) {
    node_data->AddState(ax::mojom::State::kProtected);
    node_data->SetValue(base::string16(
        text().size(), gfx::RenderText::kPasswordReplacementChar));
  } else {
    node_data->SetValue(text());
  }
  node_data->AddStringAttribute(ax::mojom::StringAttribute::kPlaceholder,
                                base::UTF16ToUTF8(GetPlaceholderText()));

  const gfx::Range range = GetSelectedRange();
  node_data->AddIntAttribute(ax::mojom::IntAttribute::kTextSelStart,
                             range.start());
  node_data->AddIntAttribute(ax::mojom::IntAttribute::kTextSelEnd, range.end());
}
