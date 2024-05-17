void AXTree::ComputeSetSizePosInSetAndCache(const AXNode& node,
                                            const AXNode* ordered_set) {
  DCHECK(ordered_set);
  std::vector<const AXNode*> items;

  bool node_is_radio_button =
      (node.data().role == ax::mojom::Role::kRadioButton);

  PopulateOrderedSetItems(ordered_set, ordered_set, items,
                          node_is_radio_button);

  int32_t num_elements = 0;

  int32_t largest_assigned_set_size = 0;

  for (size_t i = 0; i < items.size(); ++i) {
    const AXNode* item = items[i];
    ordered_set_info_map_[item->id()] = OrderedSetInfo();
    int32_t pos_in_set_value = 0;

    pos_in_set_value = num_elements + 1;

    pos_in_set_value =
        std::max(pos_in_set_value,
                 item->GetIntAttribute(ax::mojom::IntAttribute::kPosInSet));

    ordered_set_info_map_[item->id()].pos_in_set = pos_in_set_value;
    num_elements = pos_in_set_value;

    if (item->HasIntAttribute(ax::mojom::IntAttribute::kSetSize))
      largest_assigned_set_size =
          std::max(largest_assigned_set_size,
                   item->GetIntAttribute(ax::mojom::IntAttribute::kSetSize));
  }


  int32_t ordered_set_candidate =
      ordered_set->GetIntAttribute(ax::mojom::IntAttribute::kSetSize);

  int32_t set_size_value = std::max(
      std::max(num_elements, largest_assigned_set_size), ordered_set_candidate);

  if (ordered_set_info_map_.find(ordered_set->id()) ==
      ordered_set_info_map_.end())
    ordered_set_info_map_[ordered_set->id()] = OrderedSetInfo();

  if (node.SetRoleMatchesItemRole(ordered_set) || ordered_set == &node)
    ordered_set_info_map_[ordered_set->id()].set_size = set_size_value;

  for (size_t j = 0; j < items.size(); ++j) {
    const AXNode* item = items[j];
    ordered_set_info_map_[item->id()].set_size = set_size_value;
  }
}
