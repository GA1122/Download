void AutomationInternalCustomBindings::GetFocus(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() != 1 || !args[0]->IsNumber()) {
    ThrowInvalidArgumentsException(this);
    return;
  }

  int tree_id = args[0]->Int32Value();
  TreeCache* cache = GetTreeCacheFromTreeID(tree_id);
  if (!cache)
    return;

  TreeCache* focused_tree_cache = nullptr;
  ui::AXNode* focused_node = nullptr;
  if (!GetFocusInternal(cache, &focused_tree_cache, &focused_node))
    return;

  v8::Isolate* isolate = GetIsolate();
  v8::Local<v8::Object> result(v8::Object::New(isolate));
  result->Set(CreateV8String(isolate, "treeId"),
              v8::Integer::New(isolate, focused_tree_cache->tree_id));
  result->Set(CreateV8String(isolate, "nodeId"),
              v8::Integer::New(isolate, focused_node->id()));
  args.GetReturnValue().Set(result);
}
