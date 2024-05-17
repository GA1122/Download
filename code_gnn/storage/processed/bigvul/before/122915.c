bool RenderProcessHost::ShouldTryToUseExistingProcessHost(
    BrowserContext* browser_context, const GURL& url) {
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kEnableStrictSiteIsolation) ||
      command_line.HasSwitch(switches::kSitePerProcess))
    return false;

  if (run_renderer_in_process())
    return true;

  if (g_all_hosts.Get().size() >= GetMaxRendererProcessCount())
    return true;

  return GetContentClient()->browser()->
      ShouldTryToUseExistingProcessHost(browser_context, url);
}
