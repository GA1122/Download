void AutomationInternalCustomBindings::SendNodesRemovedEvent(
    ui::AXTree* tree,
    const std::vector<int>& ids) {
  auto iter = axtree_to_tree_cache_map_.find(tree);
  if (iter == axtree_to_tree_cache_map_.end())
    return;

  int tree_id = iter->second->tree_id;

  v8::Isolate* isolate = GetIsolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());
  v8::Local<v8::Array> args(v8::Array::New(GetIsolate(), 2U));
  args->Set(0U, v8::Integer::New(GetIsolate(), tree_id));
  v8::Local<v8::Array> nodes(v8::Array::New(GetIsolate(), ids.size()));
  args->Set(1U, nodes);
  for (size_t i = 0; i < ids.size(); ++i)
    nodes->Set(i, v8::Integer::New(GetIsolate(), ids[i]));
  context()->DispatchEvent("automationInternal.onNodesRemoved", args);
}
