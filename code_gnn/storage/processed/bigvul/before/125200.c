void RenderMessageFilter::OnDidCreateOutOfProcessPepperInstance(
    int plugin_child_id,
    int32 pp_instance,
    PepperRendererInstanceData instance_data,
    bool is_external) {
  DCHECK(instance_data.render_process_id == 0);
  instance_data.render_process_id = render_process_id_;
  if (is_external) {
    BrowserPpapiHostImpl* host = static_cast<BrowserPpapiHostImpl*>(
        GetContentClient()->browser()->GetExternalBrowserPpapiHost(
            plugin_child_id));
    if (host)
      host->AddInstance(pp_instance, instance_data);
  } else {
    PpapiPluginProcessHost::DidCreateOutOfProcessInstance(
        plugin_child_id, pp_instance, instance_data);
  }
}
