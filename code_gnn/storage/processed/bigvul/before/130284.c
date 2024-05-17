void DataObjectImpl::OnDownloadCompleted(const base::FilePath& file_path) {
  DataObjectImpl::StoredData::iterator iter = contents_.begin();
  for (; iter != contents_.end(); ++iter) {
    if ((*iter)->format_etc.cfFormat == CF_HDROP) {
      if ((*iter)->owns_medium) {
        ReleaseStgMedium((*iter)->medium);
        delete (*iter)->medium;
      }

      (*iter)->owns_medium = true;
      (*iter)->medium = GetStorageForFileName(file_path);

      break;
    }
  }
  DCHECK(iter != contents_.end());
}
