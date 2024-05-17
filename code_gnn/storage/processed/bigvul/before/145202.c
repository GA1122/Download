void WebstoreBindings::OnInlineWebstoreInstallResponse(
    int install_id,
    bool success,
    const std::string& error,
    webstore_install::Result result) {
  v8::Isolate* isolate = context()->isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());
  v8::Local<v8::Value> argv[] = {
    v8::Integer::New(isolate, install_id),
    v8::Boolean::New(isolate, success),
    v8::String::NewFromUtf8(isolate, error.c_str()),
    v8::String::NewFromUtf8(
        isolate, api::webstore::kInstallResultCodes[static_cast<int>(result)])
  };
  context()->module_system()->CallModuleMethod(
      "webstore", "onInstallResponse", arraysize(argv), argv);
}
