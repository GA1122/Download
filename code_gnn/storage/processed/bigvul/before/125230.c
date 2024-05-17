RenderMessageFilter::RenderMessageFilter(
    int render_process_id,
    PluginServiceImpl* plugin_service,
    BrowserContext* browser_context,
    net::URLRequestContextGetter* request_context,
    RenderWidgetHelper* render_widget_helper,
    MediaInternals* media_internals,
    DOMStorageContextImpl* dom_storage_context)
    : resource_dispatcher_host_(ResourceDispatcherHostImpl::Get()),
      plugin_service_(plugin_service),
      profile_data_directory_(browser_context->GetPath()),
      request_context_(request_context),
      resource_context_(browser_context->GetResourceContext()),
      render_widget_helper_(render_widget_helper),
      incognito_(browser_context->IsOffTheRecord()),
      dom_storage_context_(dom_storage_context),
      render_process_id_(render_process_id),
      cpu_usage_(0),
      media_internals_(media_internals) {
  DCHECK(request_context_);

  render_widget_helper_->Init(render_process_id_, resource_dispatcher_host_);
}
