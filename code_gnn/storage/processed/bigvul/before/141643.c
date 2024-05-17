bool AXTree::Unserialize(const AXTreeUpdate& update) {
  AXTreeUpdateState update_state;
  int32_t old_root_id = root_ ? root_->id() : 0;

  for (size_t i = 0; i < update.nodes.size(); ++i)
    update_state.changed_node_ids.insert(update.nodes[i].id);

  if (update.has_tree_data)
    UpdateData(update.tree_data);

  bool root_updated = false;
  if (update.node_id_to_clear != 0) {
    AXNode* node = GetFromId(update.node_id_to_clear);

    if (node && node == root_) {
      if (update.root_id != old_root_id) {
        AXNode* old_root = root_;
        root_ = nullptr;
        DestroySubtree(old_root, &update_state);
      } else {
        root_updated = true;
      }
    }

    if (node && root_ && (node != root_ || root_updated)) {
      for (int i = 0; i < node->child_count(); ++i)
        DestroySubtree(node->ChildAtIndex(i), &update_state);
      std::vector<AXNode*> children;
      node->SwapChildren(children);
      update_state.pending_nodes.insert(node);
    }
  }

  bool root_exists = GetFromId(update.root_id) != nullptr;
  for (size_t i = 0; i < update.nodes.size(); ++i) {
    bool is_new_root = !root_exists && update.nodes[i].id == update.root_id;
    if (!UpdateNode(update.nodes[i], is_new_root, &update_state))
      return false;
  }

  if (!root_) {
    error_ = "Tree has no root.";
    return false;
  }

  if (!update_state.pending_nodes.empty()) {
    error_ = "Nodes left pending by the update:";
    for (const AXNode* pending : update_state.pending_nodes)
      error_ += base::StringPrintf(" %d", pending->id());
    return false;
  }

  std::set<int32_t> table_ids_checked;
  for (size_t i = 0; i < update.nodes.size(); ++i) {
    AXNode* node = GetFromId(update.nodes[i].id);
    while (node) {
      if (table_ids_checked.find(node->id()) != table_ids_checked.end())
        break;
      const auto& table_info_entry = table_info_map_.find(node->id());
      if (table_info_entry != table_info_map_.end())
        table_info_entry->second->Invalidate();
      table_ids_checked.insert(node->id());
      node = node->parent();
    }
  }

  std::set<const AXNode*>& new_nodes = update_state.new_nodes;
  std::vector<AXTreeObserver::Change> changes;
  changes.reserve(update.nodes.size());
  for (size_t i = 0; i < update.nodes.size(); ++i) {
    AXNode* node = GetFromId(update.nodes[i].id);
    if (!node)
      continue;

    bool is_new_node = update_state.IsNewNode(node);
    bool is_reparented_node = update_state.IsReparentedNode(node);

    AXTreeObserver::ChangeType change = AXTreeObserver::NODE_CHANGED;
    if (is_new_node) {
      if (is_reparented_node) {
        bool is_subtree = !node->parent() ||
                          new_nodes.find(node->parent()) == new_nodes.end() ||
                          (node->parent() == root_ && root_updated);
        change = is_subtree ? AXTreeObserver::SUBTREE_REPARENTED
                            : AXTreeObserver::NODE_REPARENTED;
      } else {
        bool is_subtree = !node->parent() ||
                          new_nodes.find(node->parent()) == new_nodes.end() ||
                          update_state.IsRemovedNode(node->parent()) ||
                          (node->parent() == root_ && root_updated);
        change = is_subtree ? AXTreeObserver::SUBTREE_CREATED
                            : AXTreeObserver::NODE_CREATED;
      }
    }
    changes.push_back(AXTreeObserver::Change(node, change));
  }
  for (AXTreeObserver& observer : observers_) {
    observer.OnAtomicUpdateFinished(this, root_->id() != old_root_id, changes);
  }

  ordered_set_info_map_.clear();

  return true;
}
