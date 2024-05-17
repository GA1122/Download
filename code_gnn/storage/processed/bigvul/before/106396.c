 void PluginSelectionPolicy::StartInit() {
  BrowserThread::PostTask(
      BrowserThread::FILE, FROM_HERE,
      NewRunnableMethod(this, &chromeos::PluginSelectionPolicy::Init));
}
