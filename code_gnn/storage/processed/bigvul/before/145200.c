void WebstoreBindings::OnInlineInstallDownloadProgress(int percent_downloaded) {
  v8::Isolate* isolate = context()->isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());
  v8::Local<v8::Value> argv[] = {
      v8::Number::New(isolate, percent_downloaded / 100.0)};
  context()->module_system()->CallModuleMethod(
      "webstore", "onDownloadProgress", arraysize(argv), argv);
}
