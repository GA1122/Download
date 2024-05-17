bool PluginServiceImpl::IsPluginUnstable(const FilePath& path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  std::map<FilePath, std::vector<base::Time> >::const_iterator i =
      crash_times_.find(path);
  if (i == crash_times_.end()) {
    return false;
  }
  if (i->second.size() != kMaxCrashesPerInterval) {
    return false;
  }
  base::TimeDelta delta = base::Time::Now() - i->second[0];
  if (delta.InSeconds() <= kCrashesInterval) {
    return true;
  }
  return false;
}
