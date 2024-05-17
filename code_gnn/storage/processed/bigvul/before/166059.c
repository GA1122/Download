void FileSystemManagerImpl::DidCreateSnapshot(
    CreateSnapshotFileCallback callback,
    const storage::FileSystemURL& url,
    base::File::Error result,
    const base::File::Info& info,
    const base::FilePath& platform_path,
    scoped_refptr<storage::ShareableFileReference>  ) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (result != base::File::FILE_OK) {
    std::move(callback).Run(base::File::Info(), base::FilePath(), result,
                            nullptr);
    return;
  }

  scoped_refptr<storage::ShareableFileReference> file_ref =
      storage::ShareableFileReference::Get(platform_path);
  if (!security_policy_->CanReadFile(process_id_, platform_path)) {
    security_policy_->GrantReadFile(process_id_, platform_path);

    if (!file_ref.get()) {
      file_ref = storage::ShareableFileReference::GetOrCreate(
          platform_path,
          storage::ShareableFileReference::DONT_DELETE_ON_FINAL_RELEASE,
          context_->default_file_task_runner());
    }
    file_ref->AddFinalReleaseCallback(
        base::BindOnce(&RevokeFilePermission, process_id_));
  }

  if (file_ref.get()) {
    int request_id = in_transit_snapshot_files_.Add(file_ref);
    blink::mojom::ReceivedSnapshotListenerPtr listener_ptr;
    snapshot_listeners_.AddBinding(
        std::make_unique<ReceivedSnapshotListenerImpl>(request_id, this),
        mojo::MakeRequest<blink::mojom::ReceivedSnapshotListener>(
            &listener_ptr));
    std::move(callback).Run(info, platform_path, result,
                            std::move(listener_ptr));
    return;
  }

  std::move(callback).Run(info, platform_path, result, nullptr);
}
