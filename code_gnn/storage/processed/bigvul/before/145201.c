void WebstoreBindings::OnInlineInstallStageChanged(int stage) {
  const char* stage_string = NULL;
  api::webstore::InstallStage install_stage =
      static_cast<api::webstore::InstallStage>(stage);
  switch (install_stage) {
    case api::webstore::INSTALL_STAGE_DOWNLOADING:
      stage_string = api::webstore::kInstallStageDownloading;
      break;
    case api::webstore::INSTALL_STAGE_INSTALLING:
      stage_string = api::webstore::kInstallStageInstalling;
      break;
  }
  v8::Isolate* isolate = context()->isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Context::Scope context_scope(context()->v8_context());
  v8::Local<v8::Value> argv[] = {
      v8::String::NewFromUtf8(isolate, stage_string)};
  context()->module_system()->CallModuleMethod(
      "webstore", "onInstallStageChanged", arraysize(argv), argv);
}
