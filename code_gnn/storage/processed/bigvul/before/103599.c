void ChromeContentBrowserClient::BrowserRenderProcessHostCreated(
    BrowserRenderProcessHost* host) {
  int id = host->id();
  Profile* profile = host->profile();
  host->channel()->AddFilter(new ChromeRenderMessageFilter(
      id, profile, profile->GetRequestContextForRenderProcess(id)));
  host->channel()->AddFilter(new PrintingMessageFilter());
  host->channel()->AddFilter(
      new SearchProviderInstallStateMessageFilter(id, profile));
  host->channel()->AddFilter(new SpellCheckMessageFilter(id));
#if defined(OS_MACOSX)
  host->channel()->AddFilter(new TextInputClientMessageFilter(host->id()));
#endif

  host->Send(new ViewMsg_SetIsIncognitoProcess(profile->IsOffTheRecord()));
}
