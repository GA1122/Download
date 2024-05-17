void Browser::FileSelected(const FilePath& path, int index, void* params) {
  FileSelectedWithExtraInfo(ui::SelectedFileInfo(path, path), index, params);
}
