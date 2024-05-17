void AutomationInternalCustomBindings::GetState(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = GetIsolate();
  if (args.Length() < 2 || !args[0]->IsNumber() || !args[1]->IsNumber())
    ThrowInvalidArgumentsException(this);

  int tree_id = args[0]->Int32Value();
  int node_id = args[1]->Int32Value();

  TreeCache* cache = GetTreeCacheFromTreeID(tree_id);
  if (!cache)
    return;

  ui::AXNode* node = cache->tree.GetFromId(node_id);
  if (!node)
    return;

  v8::Local<v8::Object> state(v8::Object::New(isolate));
  uint32_t state_pos = 0, state_shifter = node->data().state;
  while (state_shifter) {
    if (state_shifter & 1) {
      std::string key = ToString(static_cast<ui::AXState>(state_pos));
      state->Set(CreateV8String(isolate, key), v8::Boolean::New(isolate, true));
    }
    state_shifter = state_shifter >> 1;
    state_pos++;
  }

  TreeCache* top_cache = GetTreeCacheFromTreeID(0);
  if (!top_cache)
    top_cache = cache;
  TreeCache* focused_cache = nullptr;
  ui::AXNode* focused_node = nullptr;
  if (GetFocusInternal(top_cache, &focused_cache, &focused_node)) {
    if (focused_cache == cache && focused_node == node) {
      state->Set(CreateV8String(isolate, "focused"),
                 v8::Boolean::New(isolate, true));
    }
  }
  if (cache->tree.data().focus_id == node->id()) {
    state->Set(CreateV8String(isolate, "focused"),
               v8::Boolean::New(isolate, true));
  }

  args.GetReturnValue().Set(state);
}
