void WebContext::setCachePath(const QUrl& url) {
  DCHECK(!IsInitialized());
  DCHECK(url.isLocalFile() || url.isEmpty());
  construct_props_->cache_path =
      base::FilePath(url.toLocalFile().toStdString());
}
