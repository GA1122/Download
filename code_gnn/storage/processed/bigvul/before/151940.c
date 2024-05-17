  void FileSelected(std::vector<blink::mojom::FileChooserFileInfoPtr> files,
                    const base::FilePath& base_dir,
                    blink::mojom::FileChooserParams::Mode mode) {
    proxy_ = nullptr;
    if (!render_frame_host_)
      return;
    storage::FileSystemContext* file_system_context = nullptr;
    const int pid = render_frame_host_->GetProcess()->GetID();
    auto* policy = ChildProcessSecurityPolicyImpl::GetInstance();
    for (const auto& file : files) {
      if (mode == blink::mojom::FileChooserParams::Mode::kSave) {
        policy->GrantCreateReadWriteFile(pid,
                                         file->get_native_file()->file_path);
      } else {
        if (file->is_file_system()) {
          if (!file_system_context) {
            file_system_context =
                BrowserContext::GetStoragePartition(
                    render_frame_host_->GetProcess()->GetBrowserContext(),
                    render_frame_host_->GetSiteInstance())
                    ->GetFileSystemContext();
          }
          policy->GrantReadFileSystem(
              pid, file_system_context->CrackURL(file->get_file_system()->url)
                       .mount_filesystem_id());
        } else {
          policy->GrantReadFile(pid, file->get_native_file()->file_path);
        }
      }
    }
    std::move(callback_).Run(
        FileChooserResult::New(std::move(files), base_dir));
  }
