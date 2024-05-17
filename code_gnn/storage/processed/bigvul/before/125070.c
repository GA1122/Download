bool RenderProcessHostImpl::ShouldUseProcessPerSite(
    BrowserContext* browser_context,
    const GURL& url) {

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kProcessPerSite))
    return true;


  if (GetContentClient()->browser()->
          ShouldUseProcessPerSite(browser_context, url)) {
    return true;
  }

  if (WebUIControllerFactoryRegistry::GetInstance()->UseWebUIForURL(
          browser_context, url) &&
      !url.SchemeIs(chrome::kChromeDevToolsScheme)) {
    return true;
  }

  return false;
}