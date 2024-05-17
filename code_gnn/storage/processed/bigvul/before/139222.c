bool RenderProcessHost::ShouldUseProcessPerSite(BrowserContext* browser_context,
                                                const GURL& url) {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kProcessPerSite))
    return true;

  if (WebUIControllerFactoryRegistry::GetInstance()->UseWebUIForURL(
          browser_context, url) &&
      !url.SchemeIs(kChromeDevToolsScheme)) {
    return true;
  }

  return GetContentClient()->browser()->ShouldUseProcessPerSite(browser_context,
                                                                url);
}
