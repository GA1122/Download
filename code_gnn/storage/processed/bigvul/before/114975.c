void TestingAutomationProvider::IsMenuCommandEnabled(int browser_handle,
                                                     int message_num,
                                                     bool* menu_item_enabled) {
  *menu_item_enabled = false;
  if (browser_tracker_->ContainsHandle(browser_handle)) {
    Browser* browser = browser_tracker_->GetResource(browser_handle);
    *menu_item_enabled =
        browser->command_updater()->IsCommandEnabled(message_num);
  }
}
