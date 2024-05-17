SendTabToSelfEntry::SendTabToSelfEntry(
    const std::string& guid,
    const GURL& url,
    const std::string& title,
    base::Time shared_time,
    base::Time original_navigation_time,
    const std::string& device_name,
    const std::string& target_device_sync_cache_guid)
    : guid_(guid),
      url_(url),
      title_(title),
      device_name_(device_name),
      target_device_sync_cache_guid_(target_device_sync_cache_guid),
      shared_time_(shared_time),
      original_navigation_time_(original_navigation_time),
      notification_dismissed_(false) {
  DCHECK(!guid_.empty());
  DCHECK(url_.is_valid());
  DCHECK(base::IsStringUTF8(guid_));
  DCHECK(base::IsStringUTF8(title_));
  DCHECK(base::IsStringUTF8(target_device_sync_cache_guid_));
  DCHECK(base::IsStringUTF8(device_name_));
}
