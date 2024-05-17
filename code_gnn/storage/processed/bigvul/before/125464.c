void GDataFileSystem::SetHideHostedDocuments(bool hide) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (hide == hide_hosted_docs_)
    return;

  hide_hosted_docs_ = hide;
  const FilePath root_path = directory_service_->root()->GetFilePath();

  FOR_EACH_OBSERVER(GDataFileSystemInterface::Observer, observers_,
                    OnDirectoryChanged(root_path));
}
