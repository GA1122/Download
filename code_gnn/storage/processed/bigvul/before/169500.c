void ModuleSystem::ClobberExistingNativeHandler(const std::string& name) {
  NativeHandlerMap::iterator existing_handler = native_handler_map_.find(name);
  if (existing_handler != native_handler_map_.end()) {
    clobbered_native_handlers_.push_back(std::move(existing_handler->second));
    native_handler_map_.erase(existing_handler);
  }
}
