void WebContentsImpl::SetUserAgentOverride(const std::string& override) {
  if (GetUserAgentOverride() == override)
    return;

  renderer_preferences_.user_agent_override = override;

  RenderViewHost* host = GetRenderViewHost();
  if (host)
    host->SyncRendererPrefs();

  NavigationEntry* entry = controller_.GetVisibleEntry();
  if (IsLoading() && entry != NULL && entry->GetIsOverridingUserAgent())
    controller_.Reload(ReloadType::BYPASSING_CACHE, true);

  for (auto& observer : observers_)
    observer.UserAgentOverrideSet(override);
}
