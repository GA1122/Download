void RenderFrameHostManager::CreateProxiesForChildFrame(FrameTreeNode* child) {
  RenderFrameProxyHost* outer_delegate_proxy =
      ForInnerDelegate() ? GetProxyToOuterDelegate() : nullptr;
  for (const auto& pair : proxy_hosts_) {
    if (pair.second.get() == outer_delegate_proxy)
      continue;

    child->render_manager()->CreateRenderFrameProxy(
        pair.second->GetSiteInstance());
  }
}
