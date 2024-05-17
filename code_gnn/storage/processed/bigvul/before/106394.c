bool PluginSelectionPolicy::IsAllowed(const GURL& url,
                                      const FilePath& path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));

  DCHECK(init_from_file_finished_)
      << "Tried to check policy before policy is initialized.";

  string name = path.BaseName().value();

  PolicyMap::iterator policy_iter = policies_.find(name);
  if (policy_iter != policies_.end()) {
    Policy& policy(policy_iter->second);

    bool allow = false;

    for (Policy::iterator iter = policy.begin(); iter != policy.end(); ++iter) {
      bool policy_allow = iter->first;
      string& policy_domain = iter->second;
      if (policy_domain.empty() || url.DomainIs(policy_domain.c_str(),
                                                policy_domain.size())) {
        allow = policy_allow;
      }
    }
    return allow;
  }

  return true;
}
