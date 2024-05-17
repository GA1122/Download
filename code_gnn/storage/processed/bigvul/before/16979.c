QUrl WebContext::dataPath() const {
  base::FilePath path;
  if (IsInitialized()) {
    path = context_->GetPath();
  } else {
    path = construct_props_->data_path;
  }

  if (path.empty()) {
    return QUrl();
  }

  return QUrl::fromLocalFile(QString::fromStdString(path.value()));
}
