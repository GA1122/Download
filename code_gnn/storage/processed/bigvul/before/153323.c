void Tab::GetAccessibleNodeData(ui::AXNodeData* node_data) {
  node_data->role = ax::mojom::Role::kTab;
  node_data->AddState(ax::mojom::State::kMultiselectable);
  node_data->AddBoolAttribute(ax::mojom::BoolAttribute::kSelected,
                              IsSelected());

  base::string16 name = controller_->GetAccessibleTabName(this);
  if (!name.empty()) {
    node_data->SetName(name);
  } else {
    node_data->SetNameExplicitlyEmpty();
  }
}
