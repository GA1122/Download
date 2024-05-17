void ProfileDependencyManager::AddComponent(
    ProfileKeyedBaseFactory* component) {
  all_components_.push_back(component);
  destruction_order_.clear();
}
