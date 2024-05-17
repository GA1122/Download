void InputImeEventRouterFactory::RemoveProfile(Profile* profile) {
  if (!profile || router_map_.empty())
    return;
  auto it = router_map_.find(profile);
  if (it != router_map_.end() && it->first == profile) {
    delete it->second;
    router_map_.erase(it);
  }
}
