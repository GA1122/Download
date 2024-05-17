void ModuleSystem::OnDidAddPendingModule(
    const std::string& id,
    const std::vector<std::string>& dependencies) {
  bool module_system_managed = source_map_->Contains(id);

  gin::ModuleRegistry* registry =
      gin::ModuleRegistry::From(context_->v8_context());
  DCHECK(registry);
  for (const auto& dependency : dependencies) {
    if (registry->available_modules().count(dependency) == 0 &&
        (module_system_managed || source_map_->Contains(dependency))) {
      LoadModule(dependency);
    }
  }
  registry->AttemptToLoadMoreModules(GetIsolate());
}
