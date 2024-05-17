void ApiTestEnvironment::RunTestInner(const std::string& test_name,
                                      const base::Closure& quit_closure) {
  v8::HandleScope scope(env()->isolate());
  ModuleSystem::NativesEnabledScope natives_enabled(env()->module_system());
  v8::Local<v8::Value> result =
      env()->module_system()->CallModuleMethod("testBody", test_name);
  if (!result->IsTrue()) {
    base::MessageLoop::current()->PostTask(FROM_HERE, quit_closure);
    FAIL() << "Failed to run test \"" << test_name << "\"";
  }
}
