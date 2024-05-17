void RenderFrameDevToolsAgentHost::WebContentsCreated(
    WebContents* web_contents) {
  if (ShouldForceCreation()) {
    DevToolsAgentHost::GetOrCreateFor(web_contents);
  }
}
