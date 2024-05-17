  void OnJsExecutionDone(base::Closure callback, const base::Value* value) {
    js_result_.reset(value->DeepCopy());
    callback.Run();
  }
