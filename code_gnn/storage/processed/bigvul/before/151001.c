void DevToolsUIBindings::IndexPath(int index_request_id,
                                   const std::string& file_system_path) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  CHECK(web_contents_->GetURL().SchemeIs(content::kChromeDevToolsScheme));
  if (!file_helper_->IsFileSystemAdded(file_system_path)) {
    IndexingDone(index_request_id, file_system_path);
    return;
  }
  if (indexing_jobs_.count(index_request_id) != 0)
    return;
  indexing_jobs_[index_request_id] =
      scoped_refptr<DevToolsFileSystemIndexer::FileSystemIndexingJob>(
          file_system_indexer_->IndexPath(
              file_system_path,
              Bind(&DevToolsUIBindings::IndexingTotalWorkCalculated,
                   weak_factory_.GetWeakPtr(),
                   index_request_id,
                   file_system_path),
              Bind(&DevToolsUIBindings::IndexingWorked,
                   weak_factory_.GetWeakPtr(),
                   index_request_id,
                   file_system_path),
              Bind(&DevToolsUIBindings::IndexingDone,
                   weak_factory_.GetWeakPtr(),
                   index_request_id,
                   file_system_path)));
}
