void AutomationInternalCustomBindings::SendTreeChangeEvent(
    api::automation::TreeChangeType change_type,
    ui::AXTree* tree,
    ui::AXNode* node) {
  if (!is_active_profile_)
    return;

  if (node->data().HasIntAttribute(ui::AX_ATTR_CHILD_TREE_ID))
    SendChildTreeIDEvent(tree, node);

  switch (tree_change_observer_overall_filter_) {
    case api::automation::TREE_CHANGE_OBSERVER_FILTER_NOTREECHANGES:
    default:
      return;
    case api::automation::TREE_CHANGE_OBSERVER_FILTER_LIVEREGIONTREECHANGES:
      if (!node->data().HasStringAttribute(ui::AX_ATTR_CONTAINER_LIVE_STATUS) &&
          node->data().role != ui::AX_ROLE_ALERT) {
        return;
      }
      break;
    case api::automation::TREE_CHANGE_OBSERVER_FILTER_ALLTREECHANGES:
      break;
  }

  auto iter = axtree_to_tree_cache_map_.find(tree);
  if (iter == axtree_to_tree_cache_map_.end())
    return;

  int tree_id = iter->second->tree_id;

  v8::Isolate* isolate = GetIsolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());

  for (const auto& observer : tree_change_observers_) {
    switch (observer.filter) {
      case api::automation::TREE_CHANGE_OBSERVER_FILTER_NOTREECHANGES:
      default:
        continue;
      case api::automation::TREE_CHANGE_OBSERVER_FILTER_LIVEREGIONTREECHANGES:
        if (!node->data().HasStringAttribute(
                ui::AX_ATTR_CONTAINER_LIVE_STATUS) &&
            node->data().role != ui::AX_ROLE_ALERT) {
          continue;
        }
        break;
      case api::automation::TREE_CHANGE_OBSERVER_FILTER_ALLTREECHANGES:
        break;
    }

    v8::Local<v8::Array> args(v8::Array::New(GetIsolate(), 4U));
    args->Set(0U, v8::Integer::New(GetIsolate(), observer.id));
    args->Set(1U, v8::Integer::New(GetIsolate(), tree_id));
    args->Set(2U, v8::Integer::New(GetIsolate(), node->id()));
    args->Set(3U, CreateV8String(isolate, ToString(change_type)));
    context()->DispatchEvent("automationInternal.onTreeChange", args);
  }
}
