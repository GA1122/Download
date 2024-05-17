void AXTree::UpdateReverseRelations(AXNode* node, const AXNodeData& new_data) {
  const AXNodeData& old_data = node->data();
  int id = new_data.id;
  auto int_callback = [this, id](ax::mojom::IntAttribute attr,
                                 const int& old_id, const int& new_id) {
    if (!IsNodeIdIntAttribute(attr))
      return;

    auto& map = int_reverse_relations_[attr];
    if (map.find(old_id) != map.end()) {
      map[old_id].erase(id);
      if (map[old_id].empty())
        map.erase(old_id);
    }

    if (new_id)
      map[new_id].insert(id);
  };
  CallIfAttributeValuesChanged(old_data.int_attributes, new_data.int_attributes,
                               0, int_callback);

  auto intlist_callback = [this, id](ax::mojom::IntListAttribute attr,
                                     const std::vector<int32_t>& old_idlist,
                                     const std::vector<int32_t>& new_idlist) {
    if (!IsNodeIdIntListAttribute(attr))
      return;

    auto& map = intlist_reverse_relations_[attr];
    for (int32_t old_id : old_idlist) {
      if (map.find(old_id) != map.end()) {
        map[old_id].erase(id);
        if (map[old_id].empty())
          map.erase(old_id);
      }
    }
    for (int32_t new_id : new_idlist)
      intlist_reverse_relations_[attr][new_id].insert(id);
  };
  CallIfAttributeValuesChanged(old_data.intlist_attributes,
                               new_data.intlist_attributes,
                               std::vector<int32_t>(), intlist_callback);

  auto string_callback = [this, id](ax::mojom::StringAttribute attr,
                                    const std::string& old_string,
                                    const std::string& new_string) {
    if (attr == ax::mojom::StringAttribute::kChildTreeId) {
      AXTreeID old_ax_tree_id = AXTreeID::FromString(old_string);
      if (child_tree_id_reverse_map_.find(old_ax_tree_id) !=
          child_tree_id_reverse_map_.end()) {
        child_tree_id_reverse_map_[old_ax_tree_id].erase(id);
        if (child_tree_id_reverse_map_[old_ax_tree_id].empty())
          child_tree_id_reverse_map_.erase(old_ax_tree_id);
      }

      if (!new_string.empty()) {
        AXTreeID new_ax_tree_id = AXTreeID::FromString(new_string);
        child_tree_id_reverse_map_[new_ax_tree_id].insert(id);
      }
    }
  };

  CallIfAttributeValuesChanged(old_data.string_attributes,
                               new_data.string_attributes, std::string(),
                               string_callback);
}
