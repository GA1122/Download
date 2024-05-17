void AutomationInternalCustomBindings::OnAccessibilityEvent(
    const ExtensionMsg_AccessibilityEventParams& params,
    bool is_active_profile) {
  is_active_profile_ = is_active_profile;
  int tree_id = params.tree_id;
  TreeCache* cache;
  auto iter = tree_id_to_tree_cache_map_.find(tree_id);
  if (iter == tree_id_to_tree_cache_map_.end()) {
    cache = new TreeCache();
    cache->tab_id = -1;
    cache->tree_id = params.tree_id;
    cache->parent_node_id_from_parent_tree = -1;
    cache->tree.SetDelegate(this);
    cache->owner = this;
    tree_id_to_tree_cache_map_.insert(std::make_pair(tree_id, cache));
    axtree_to_tree_cache_map_.insert(std::make_pair(&cache->tree, cache));
  } else {
    cache = iter->second;
  }

  cache->location_offset = params.location_offset;
  deleted_node_ids_.clear();
  if (!cache->tree.Unserialize(params.update)) {
    LOG(ERROR) << cache->tree.error();
    return;
  }

  if (!is_active_profile)
    return;

  SendNodesRemovedEvent(&cache->tree, deleted_node_ids_);
  deleted_node_ids_.clear();

  v8::Isolate* isolate = GetIsolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());
  v8::Local<v8::Array> args(v8::Array::New(GetIsolate(), 1U));
  v8::Local<v8::Object> event_params(v8::Object::New(GetIsolate()));
  event_params->Set(CreateV8String(isolate, "treeID"),
                    v8::Integer::New(GetIsolate(), params.tree_id));
  event_params->Set(CreateV8String(isolate, "targetID"),
                    v8::Integer::New(GetIsolate(), params.id));
  event_params->Set(CreateV8String(isolate, "eventType"),
                    CreateV8String(isolate, ToString(params.event_type)));
  args->Set(0U, event_params);
  context()->DispatchEvent("automationInternal.onAccessibilityEvent", args);
}
