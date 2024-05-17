void DownloadItemImpl::ReleaseDownloadFile(bool destroy_file) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DVLOG(20) << __func__ << "() destroy_file:" << destroy_file;

  if (destroy_file) {
    GetDownloadTaskRunner()->PostTask(
        FROM_HERE,
        base::BindOnce(&DownloadFileCancel, base::Passed(&download_file_)));
    destination_info_.current_path.clear();
    received_slices_.clear();
  } else {
    GetDownloadTaskRunner()->PostTask(
        FROM_HERE, base::BindOnce(base::IgnoreResult(&DownloadFileDetach),
                                  base::Passed(&download_file_)));
  }
  weak_ptr_factory_.InvalidateWeakPtrs();
}
