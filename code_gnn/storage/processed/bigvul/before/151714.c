void Browser::OpenFile() {
  content::RecordAction(UserMetricsAction("OpenFile"));
  select_file_dialog_ = ui::SelectFileDialog::Create(
      this, new ChromeSelectFilePolicy(
          tab_strip_model_->GetActiveWebContents()));

  const base::FilePath directory = profile_->last_selected_directory();

  gfx::NativeWindow parent_window = window_->GetNativeWindow();
  ui::SelectFileDialog::FileTypeInfo file_types;
  file_types.allowed_paths = ui::SelectFileDialog::FileTypeInfo::ANY_PATH;
  select_file_dialog_->SelectFile(ui::SelectFileDialog::SELECT_OPEN_FILE,
                                  base::string16(),
                                  directory,
                                  &file_types,
                                  0,
                                  base::FilePath::StringType(),
                                  parent_window,
                                  NULL);
}
