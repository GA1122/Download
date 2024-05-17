  void AbortCompilation() {
    builder_.Abort(V8ThrowException::CreateTypeError(
        script_state_->GetIsolate(), "Could not download wasm module"));
  }
