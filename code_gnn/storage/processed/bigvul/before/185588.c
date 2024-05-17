  SynchronousCompositorImpl* SynchronousCompositorImpl::FromID(int process_id,
                                                               int routing_id) {
  if (g_factory == NULL)
    return NULL;
//   if (g_factory == nullptr)
//     return nullptr;
    RenderViewHost* rvh = RenderViewHost::FromID(process_id, routing_id);
    if (!rvh)
    return NULL;
//     return nullptr;
    WebContents* contents = WebContents::FromRenderViewHost(rvh);
    if (!contents)
    return NULL;
//     return nullptr;
    return FromWebContents(contents);
  }