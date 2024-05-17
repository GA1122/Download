void PluginServiceImpl::RegisterPluginCrash(const FilePath& path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  std::map<FilePath, std::vector<base::Time> >::iterator i =
      crash_times_.find(path);
  if (i == crash_times_.end()) {
    crash_times_[path] = std::vector<base::Time>();
    i = crash_times_.find(path);
  }
  if (i->second.size() == kMaxCrashesPerInterval) {
    i->second.erase(i->second.begin());
  }
  base::Time time = base::Time::Now();
  i->second.push_back(time);
}
