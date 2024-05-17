void DownloadItemImpl::MaybeCompleteDownload() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!IsSavePackageDownload());

  if (!IsDownloadReadyForCompletion(
          base::Bind(&DownloadItemImpl::MaybeCompleteDownload,
                     weak_ptr_factory_.GetWeakPtr())))
    return;
  DCHECK_EQ(IN_PROGRESS_INTERNAL, state_);
  DCHECK(!IsDangerous());
  DCHECK(AllDataSaved());

  OnDownloadCompleting();
}
