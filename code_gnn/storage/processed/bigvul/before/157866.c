void WebContentsImpl::SetUserAgentOverride(const std::string& override,
                                           bool override_in_new_tabs) {
  if (GetUserAgentOverride() == override)
    return;

  should_override_user_agent_in_new_tabs_ = override_in_new_tabs;

  renderer_preferences_.user_agent_override = override;

  RenderViewHost* host = GetRenderViewHost();
  if (host)
    host->SyncRendererPrefs();

  NavigationEntry* entry = controller_.GetVisibleEntry();
  if (IsLoading() && entry != nullptr && entry->GetIsOverridingUserAgent())
    controller_.Reload(ReloadType::BYPASSING_CACHE, true);

  for (auto& observer : observers_)
    observer.UserAgentOverrideSet(override);
}
