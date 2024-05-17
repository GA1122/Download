void Browser::OpenFile() {
  UserMetrics::RecordAction(UserMetricsAction("OpenFile"), profile_);
#if defined(OS_CHROMEOS)
  FileBrowseUI::OpenPopup(profile_,
                          "",
                          FileBrowseUI::kPopupWidth,
                          FileBrowseUI::kPopupHeight);
#else
  if (!select_file_dialog_.get())
    select_file_dialog_ = SelectFileDialog::Create(this);

  const FilePath directory = profile_->last_selected_directory();

  gfx::NativeWindow parent_window = window_->GetNativeHandle();
  select_file_dialog_->SelectFile(SelectFileDialog::SELECT_OPEN_FILE,
                                  string16(), directory,
                                  NULL, 0, FILE_PATH_LITERAL(""),
                                  parent_window, NULL);
#endif
}
