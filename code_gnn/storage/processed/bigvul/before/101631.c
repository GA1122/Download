void Browser::EnumerateDirectory(TabContents* tab, int request_id,
                                 const FilePath& path) {
  EnumerateDirectoryHelper(tab, request_id, path);
}
