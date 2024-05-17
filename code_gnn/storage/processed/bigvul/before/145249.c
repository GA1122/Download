void Dispatcher::OnUserScriptsUpdated(const std::set<HostID>& changed_hosts,
                                      const std::vector<UserScript*>& scripts) {
  UpdateActiveExtensions();
}
