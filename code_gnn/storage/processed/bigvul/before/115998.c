string16 ExtensionGlobalError::GenerateMessage() {
  if (extension_service_.get()) {
    return
        GenerateMessageSection(external_extension_ids_.get(),
                               IDS_EXTENSION_ALERT_ITEM_EXTERNAL) +
        GenerateMessageSection(blacklisted_extension_ids_.get(),
                               IDS_EXTENSION_ALERT_ITEM_EXTERNAL) +
        GenerateMessageSection(orphaned_extension_ids_.get(),
                               IDS_EXTENSION_ALERT_ITEM_EXTERNAL);
  } else {
    return string16();
  }
}
