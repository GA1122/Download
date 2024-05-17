void WebContentsImpl::EnsureOpenerProxiesExist(RenderFrameHost* source_rfh) {
  WebContentsImpl* source_web_contents = static_cast<WebContentsImpl*>(
      WebContents::FromRenderFrameHost(source_rfh));

  if (source_web_contents) {
    if (GetBrowserPluginEmbedder() &&
        GuestMode::IsCrossProcessFrameGuest(source_web_contents)) {
      return;
    }

    if (this != source_web_contents && GetBrowserPluginGuest()) {
      source_web_contents->GetRenderManager()->CreateRenderFrameProxy(
          GetSiteInstance());
    } else {
      RenderFrameHostImpl* source_rfhi =
          static_cast<RenderFrameHostImpl*>(source_rfh);
      source_rfhi->frame_tree_node()->render_manager()->CreateOpenerProxies(
          GetSiteInstance(), nullptr);
    }
   }
 }
