GURL DevToolsUI::GetRemoteBaseURL() {
  return GURL(base::StringPrintf(
      "%s%s/%s/",
      kRemoteFrontendBase,
      kRemoteFrontendPath,
       content::GetWebKitRevision().c_str()));
 }
