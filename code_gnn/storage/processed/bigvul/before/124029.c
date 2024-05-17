void BookmarksExportFunction::FileSelected(const base::FilePath& path,
                                           int index,
                                           void* params) {
#if !defined(OS_ANDROID)
  bookmark_html_writer::WriteBookmarks(profile(), path, NULL);
#endif
  Release();   
}
