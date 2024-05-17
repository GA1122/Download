bool Extension::IsSandboxedPage(const std::string& relative_path) const {
  return ResourceMatches(sandboxed_pages_, relative_path);
}
