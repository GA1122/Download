void ResourceDispatcherHostImpl::RegisterDownloadedTempFile(
    int child_id, int request_id, const base::FilePath& file_path) {
  scoped_refptr<ShareableFileReference> reference =
      ShareableFileReference::Get(file_path);
  DCHECK(reference.get());

  registered_temp_files_[child_id][request_id] = reference;
  ChildProcessSecurityPolicyImpl::GetInstance()->GrantReadFile(
      child_id, reference->path());

  reference->AddFinalReleaseCallback(
      base::Bind(&RemoveDownloadFileFromChildSecurityPolicy,
                 child_id));
}
