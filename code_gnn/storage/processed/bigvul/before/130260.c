bool OSExchangeDataProviderWin::GetFileContents(
    base::FilePath* filename,
    std::string* file_contents) const {
  base::string16 filename_str;
  if (!ClipboardUtil::GetFileContents(source_object_, &filename_str,
                                      file_contents)) {
    return false;
  }
  *filename = base::FilePath(filename_str);
  return true;
}
