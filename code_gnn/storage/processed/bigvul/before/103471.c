void ExtensionService::Init() {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  DCHECK(!ready_);   
  DCHECK_EQ(extensions_.size(), 0u);

  g_browser_process->resource_dispatcher_host();

  LoadAllExtensions();

  CheckForExternalUpdates();

  GarbageCollectExtensions();
}
