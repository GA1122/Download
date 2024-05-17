void SanitizeGeneratedFileName(std::string& filename) {
  if (!filename.empty()) {
    TrimString(filename, ".", &filename);
#if defined(OS_WIN)
    std::string::size_type pos = filename.find_last_not_of(" .");
    if (pos == std::string::npos)
      filename.resize(0);
    else
      filename.resize(++pos);
#endif
    ReplaceSubstringsAfterOffset(&filename, 0, "/", "_");
    ReplaceSubstringsAfterOffset(&filename, 0, "\\", "_");
  }
}
