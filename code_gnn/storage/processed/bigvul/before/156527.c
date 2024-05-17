bool ChildProcessSecurityPolicyImpl::CanReadRequestBody(
    SiteInstance* site_instance,
    const scoped_refptr<network::ResourceRequestBody>& body) {
  DCHECK(site_instance);
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  int child_id = site_instance->GetProcess()->GetID();

  StoragePartition* storage_partition = BrowserContext::GetStoragePartition(
      site_instance->GetBrowserContext(), site_instance);
  const storage::FileSystemContext* file_system_context =
      storage_partition->GetFileSystemContext();

  return CanReadRequestBody(child_id, file_system_context, body);
}
