QUrl WebContext::cachePath() const {
  base::FilePath path;
  if (IsInitialized()) {
    path = context_->GetCachePath();
  } else {
    path = construct_props_->cache_path;
  }

  if (path.empty()) {
    return QUrl();
  }

  return QUrl::fromLocalFile(QString::fromStdString(path.value()));
}
