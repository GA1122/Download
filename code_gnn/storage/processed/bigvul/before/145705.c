ApiTestEnvironment::ApiTestEnvironment(
    ModuleSystemTestEnvironment* environment) {
  env_ = environment;
  InitializeEnvironment();
  RegisterModules();
}
