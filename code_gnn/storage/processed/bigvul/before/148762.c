RenderViewHostImpl* InterstitialPageImpl::CreateRenderViewHost() {
  if (!enabled())
    return NULL;

  BrowserContext* browser_context = web_contents()->GetBrowserContext();
  scoped_refptr<SiteInstance> site_instance =
      SiteInstance::Create(browser_context);
  DOMStorageContextWrapper* dom_storage_context =
      static_cast<DOMStorageContextWrapper*>(
          BrowserContext::GetStoragePartition(
              browser_context, site_instance.get())->GetDOMStorageContext());
  session_storage_namespace_ =
      new SessionStorageNamespaceImpl(dom_storage_context);

  int32_t widget_routing_id = site_instance->GetProcess()->GetNextRoutingID();
  frame_tree_->root()->render_manager()->Init(
      site_instance.get(), widget_routing_id, MSG_ROUTING_NONE,
      widget_routing_id, false);
  return frame_tree_->root()->current_frame_host()->render_view_host();
}
