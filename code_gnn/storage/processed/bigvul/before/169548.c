  void Run(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = automation_bindings_->GetIsolate();
    if (args.Length() < 2 || !args[0]->IsNumber() || !args[1]->IsNumber())
      ThrowInvalidArgumentsException(automation_bindings_);

    int tree_id = args[0]->Int32Value();
    int node_id = args[1]->Int32Value();

    TreeCache* cache = automation_bindings_->GetTreeCacheFromTreeID(tree_id);
    if (!cache)
      return;

    ui::AXNode* node = cache->tree.GetFromId(node_id);
    if (!node)
      return;

    function_(isolate, args.GetReturnValue(), cache, node);
  }
