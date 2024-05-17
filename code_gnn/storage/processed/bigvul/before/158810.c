bool Browser::CanDragEnter(content::WebContents* source,
                           const content::DropData& data,
                           blink::WebDragOperationsMask operations_allowed) {
#if defined(OS_CHROMEOS)
  if ((operations_allowed & blink::kWebDragOperationLink) &&
      chrome::SettingsWindowManager::GetInstance()->IsSettingsBrowser(this)) {
    return false;
  }
#endif
  return true;
}
