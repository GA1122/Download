void OpenChannel(PluginProcessHost::Client* client) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  PluginServiceImpl::GetInstance()->OpenChannelToNpapiPlugin(
      0, 0, GURL(), GURL(), kNPAPITestPluginMimeType, client);
}
