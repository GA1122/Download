void UrlmonUrlRequestManager::AddPrivacyDataForUrl(
    const std::string& url, const std::string& policy_ref,
    int32 flags) {
  DCHECK(!url.empty());

  bool fire_privacy_event = false;

  if (privacy_info_.privacy_records.empty())
    flags |= PRIVACY_URLISTOPLEVEL;

  if (!privacy_info_.privacy_impacted) {
    if (flags & (COOKIEACTION_ACCEPT | COOKIEACTION_REJECT |
                 COOKIEACTION_DOWNGRADE)) {
      privacy_info_.privacy_impacted = true;
      fire_privacy_event = true;
    }
  }

  PrivacyInfo::PrivacyEntry& privacy_entry =
      privacy_info_.privacy_records[UTF8ToWide(url)];

  privacy_entry.flags |= flags;
  privacy_entry.policy_ref = UTF8ToWide(policy_ref);

  if (fire_privacy_event && IsWindow(notification_window_)) {
    PostMessage(notification_window_, WM_FIRE_PRIVACY_CHANGE_NOTIFICATION, 1,
                0);
  }
}
