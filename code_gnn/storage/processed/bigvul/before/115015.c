void TestingAutomationProvider::SavePage(int tab_handle,
                                         const FilePath& file_name,
                                         const FilePath& dir_path,
                                         int type,
                                         bool* success) {
  SavePackage::SavePackageType save_type =
      static_cast<SavePackage::SavePackageType>(type);
  if (save_type < SavePackage::SAVE_AS_ONLY_HTML ||
      save_type > SavePackage::SAVE_AS_COMPLETE_HTML) {
    *success = false;
    return;
  }

  if (!tab_tracker_->ContainsHandle(tab_handle)) {
    *success = false;
    return;
  }

  NavigationController* nav = tab_tracker_->GetResource(tab_handle);
  Browser* browser = FindAndActivateTab(nav);
  if (!browser->command_updater()->IsCommandEnabled(IDC_SAVE_PAGE)) {
    *success = false;
    return;
  }

  nav->GetWebContents()->SavePage(file_name, dir_path, save_type);
  *success = true;
}
