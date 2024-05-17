void DevToolsAgent::OnFrontendLoaded() {
  WebDevToolsAgent* web_agent = GetWebAgent();
  if (web_agent)
    web_agent->frontendLoaded();
}
