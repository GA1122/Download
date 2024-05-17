void PluginServiceImpl::GetPlugins(const GetPluginsCallback& callback) {
  scoped_refptr<base::MessageLoopProxy> target_loop(
      MessageLoop::current()->message_loop_proxy());

  if (LoadPluginListInProcess()) {
    BrowserThread::GetBlockingPool()->
        PostSequencedWorkerTaskWithShutdownBehavior(
            plugin_list_token_,
            FROM_HERE,
            base::Bind(&PluginServiceImpl::GetPluginsInternal,
                       base::Unretained(this),
                       target_loop, callback),
        base::SequencedWorkerPool::SKIP_ON_SHUTDOWN);
    return;
  }
#if defined(OS_POSIX)
  std::vector<webkit::WebPluginInfo> cached_plugins;
  if (plugin_list_->GetPluginsNoRefresh(&cached_plugins)) {
    target_loop->PostTask(FROM_HERE,
        base::Bind(callback, cached_plugins));
  } else {
    if (!plugin_loader_.get())
      plugin_loader_ = new PluginLoaderPosix;
    BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
        base::Bind(&PluginLoaderPosix::LoadPlugins, plugin_loader_,
                   target_loop, callback));
  }
#else
  NOTREACHED();
#endif
}
