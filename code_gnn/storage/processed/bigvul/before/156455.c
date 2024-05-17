GURL RenderFrameDevToolsAgentHost::GetURL() {
  WebContents* web_contents = GetWebContents();
  if (web_contents && !IsChildFrame())
    return web_contents->GetVisibleURL();
  if (frame_host_)
    return frame_host_->GetLastCommittedURL();
  return GURL();
}
