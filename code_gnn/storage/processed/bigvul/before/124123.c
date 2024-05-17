void ChromeContentBrowserClient::RenderProcessHostCreated(
    content::RenderProcessHost* host) {
  int id = host->GetID();
  Profile* profile = Profile::FromBrowserContext(host->GetBrowserContext());
  net::URLRequestContextGetter* context =
      profile->GetRequestContextForRenderProcess(id);

  host->GetChannel()->AddFilter(new ChromeRenderMessageFilter(
      id, profile, context));
#if defined(ENABLE_PLUGINS)
  host->GetChannel()->AddFilter(new PluginInfoMessageFilter(id, profile));
#endif
#if defined(ENABLE_PRINTING)
  host->GetChannel()->AddFilter(new PrintingMessageFilter(id, profile));
#endif
  host->GetChannel()->AddFilter(
      new SearchProviderInstallStateMessageFilter(id, profile));
  host->GetChannel()->AddFilter(new SpellCheckMessageFilter(id));
#if defined(OS_MACOSX)
  host->GetChannel()->AddFilter(new SpellCheckMessageFilterMac());
#endif
  host->GetChannel()->AddFilter(new ChromeNetBenchmarkingMessageFilter(
      id, profile, context));
  host->GetChannel()->AddFilter(
      new prerender::PrerenderMessageFilter(id, profile));

  host->Send(new ChromeViewMsg_SetIsIncognitoProcess(
      profile->IsOffTheRecord()));

  SendExtensionWebRequestStatusToHost(host);

  RendererContentSettingRules rules;
  GetRendererContentSettingRules(profile->GetHostContentSettingsMap(), &rules);
  host->Send(new ChromeViewMsg_SetContentSettingRules(rules));
}
