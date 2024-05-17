void WebContext::setDataPath(const QUrl& url) {
  DCHECK(!IsInitialized());
  DCHECK(url.isLocalFile() || url.isEmpty());
  construct_props_->data_path =
      base::FilePath(url.toLocalFile().toStdString());
}
