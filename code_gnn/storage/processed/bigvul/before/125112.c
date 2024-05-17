void PluginServiceImpl::AppActivated() {
  BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                          base::Bind(&NotifyPluginsOfActivation));
}
