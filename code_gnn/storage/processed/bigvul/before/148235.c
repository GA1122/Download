const base::Process& BrowserPpapiHostImpl::GetPluginProcess() const {
  DCHECK(in_process_ || plugin_process_.IsValid());
  return plugin_process_;
}
