void Textfield::SetAssociatedLabel(View* labelling_view) {
  DCHECK(labelling_view);
  label_ax_id_ = labelling_view->GetViewAccessibility().GetUniqueId().Get();
  ui::AXNodeData node_data;
  labelling_view->GetAccessibleNodeData(&node_data);
  SetAccessibleName(
      node_data.GetString16Attribute(ax::mojom::StringAttribute::kName));
}
