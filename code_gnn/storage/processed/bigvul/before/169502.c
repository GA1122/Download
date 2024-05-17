void ModuleSystem::Invalidate() {
  {
    v8::HandleScope scope(GetIsolate());
    v8::Local<v8::Object> global = context()->v8_context()->Global();
    DeletePrivate(global, kModulesField);
    DeletePrivate(global, kModuleSystem);
  }

  for (const auto& handler : native_handler_map_)
    handler.second->Invalidate();
  for (const auto& clobbered_handler : clobbered_native_handlers_)
    clobbered_handler->Invalidate();

  ObjectBackedNativeHandler::Invalidate();
}
