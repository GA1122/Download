bool BookmarksFunction::GetBookmarkIdAsInt64(const std::string& id_string,
                                             int64* id) {
  if (base::StringToInt64(id_string, id))
    return true;

  error_ = keys::kInvalidIdError;
  return false;
}
