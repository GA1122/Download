void HeadlessDevToolsManagerDelegate::SessionDestroyed(
    content::DevToolsAgentHost* agent_host,
    int session_id) {
  if (!browser_)
    return;

  content::WebContents* web_contents = agent_host->GetWebContents();
  if (!web_contents)
    return;

  HeadlessWebContentsImpl* headless_contents =
      HeadlessWebContentsImpl::From(browser_.get(), web_contents);
  if (!headless_contents)
    return;

  headless_contents->SetBeginFrameEventsEnabled(session_id, false);
}
