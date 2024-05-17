void LayoutTestContentBrowserClient::RenderProcessWillLaunch(
    RenderProcessHost* host) {
  ShellContentBrowserClient::RenderProcessWillLaunch(host);

  StoragePartition* partition =
      BrowserContext::GetDefaultStoragePartition(browser_context());
  host->AddFilter(new LayoutTestMessageFilter(
      host->GetID(),
      partition->GetDatabaseTracker(),
      partition->GetQuotaManager(),
      partition->GetURLRequestContext()));

  host->Send(new ShellViewMsg_SetWebKitSourceDir(GetWebKitRootDirFilePath()));
}
