void TabHelper::RenderFrameCreated(content::RenderFrameHost* host) {
  SetTabId(host);
}
