void PluginServiceImpl::GetPluginsInternal(
     base::MessageLoopProxy* target_loop,
     const PluginService::GetPluginsCallback& callback) {
  DCHECK(BrowserThread::GetBlockingPool()->IsRunningSequenceOnCurrentThread(
      plugin_list_token_));

  std::vector<webkit::WebPluginInfo> plugins;
  plugin_list_->GetPlugins(&plugins);

  target_loop->PostTask(FROM_HERE,
      base::Bind(callback, plugins));
}
