void Browser::FileSelected(const base::FilePath& path, int index,
                           void* params) {
  FileSelectedWithExtraInfo(ui::SelectedFileInfo(path, path), index, params);
}
