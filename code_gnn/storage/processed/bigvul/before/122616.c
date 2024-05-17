bool Extension::CheckConflictingFeatures(std::string* utf8_error) const {
  if (has_lazy_background_page() &&
      HasAPIPermission(APIPermission::kWebRequest)) {
    *utf8_error = errors::kWebRequestConflictsWithLazyBackground;
    return false;
  }

  return true;
}
