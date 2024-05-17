void TabHelper::GetApplicationInfo(WebAppAction action) {
  NavigationEntry* entry =
      web_contents()->GetController().GetLastCommittedEntry();
  if (!entry)
    return;

  pending_web_app_action_ = action;
  last_committed_nav_entry_unique_id_ = entry->GetUniqueID();

  Send(new ChromeViewMsg_GetWebApplicationInfo(routing_id()));
}
