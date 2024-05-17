void ModuleSystem::OverrideNativeHandlerForTest(const std::string& name) {
  ClobberExistingNativeHandler(name);
  overridden_native_handlers_.insert(name);
}
