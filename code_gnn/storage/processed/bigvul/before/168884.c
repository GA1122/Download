bool DebuggerFunction::InitAgentHost() {
  if (debuggee_.tab_id) {
    WebContents* web_contents = NULL;
    bool result = ExtensionTabUtil::GetTabById(*debuggee_.tab_id,
                                               GetProfile(),
                                               include_incognito(),
                                               NULL,
                                               NULL,
                                               &web_contents,
                                               NULL);
    if (result && web_contents) {
      GURL url = web_contents->GetVisibleURL();
      if (PermissionsData::IsRestrictedUrl(url, extension(), &error_))
        return false;
      agent_host_ = DevToolsAgentHost::GetOrCreateFor(web_contents);
    }
  } else if (debuggee_.extension_id) {
    ExtensionHost* extension_host =
        ProcessManager::Get(GetProfile())
            ->GetBackgroundHostForExtension(*debuggee_.extension_id);
    if (extension_host) {
      if (PermissionsData::IsRestrictedUrl(extension_host->GetURL(),
                                           extension(),
                                           &error_)) {
        return false;
      }
      agent_host_ =
          DevToolsAgentHost::GetOrCreateFor(extension_host->host_contents());
    }
  } else if (debuggee_.target_id) {
    agent_host_ = DevToolsAgentHost::GetForId(*debuggee_.target_id);
    if (agent_host_.get()) {
      if (PermissionsData::IsRestrictedUrl(agent_host_->GetURL(),
                                           extension(),
                                           &error_)) {
        agent_host_ = nullptr;
        return false;
      }
    }
  } else {
    error_ = keys::kInvalidTargetError;
    return false;
  }

  if (!agent_host_.get()) {
    FormatErrorMessage(keys::kNoTargetError);
    return false;
  }
  return true;
}
