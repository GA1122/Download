void OpenChannelAndThenCancel(PluginProcessHost::Client* client) {
  OpenChannel(client);
  PluginServiceImpl::GetInstance()->CancelOpenChannelToNpapiPlugin(client);
  BrowserThread::PostTaskAndReply(
      BrowserThread::FILE, FROM_HERE,
      base::Bind(&base::DoNothing),
      base::Bind(&QuitUIMessageLoopFromIOThread));
}
