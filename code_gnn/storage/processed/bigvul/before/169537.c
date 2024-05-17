void AutomationInternalCustomBindings::OnNodeDataWillChange(
    ui::AXTree* tree,
    const ui::AXNodeData& old_node_data,
    const ui::AXNodeData& new_node_data) {
  if (old_node_data.GetStringAttribute(ui::AX_ATTR_NAME) !=
      new_node_data.GetStringAttribute(ui::AX_ATTR_NAME))
    text_changed_node_ids_.push_back(new_node_data.id);
}
