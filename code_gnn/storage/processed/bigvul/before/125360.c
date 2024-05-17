void GDataFileSystem::GetAvailableSpaceOnUIThread(
    const GetAvailableSpaceCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  if (gdata::util::IsDriveV2ApiEnabled()) {
    documents_service_->GetAboutResource(
      base::Bind(&GDataFileSystem::OnGetAboutResource,
                 ui_weak_ptr_,
                 callback));
    return;
  }

  documents_service_->GetAccountMetadata(
      base::Bind(&GDataFileSystem::OnGetAvailableSpace,
                 ui_weak_ptr_,
                 callback));
}
