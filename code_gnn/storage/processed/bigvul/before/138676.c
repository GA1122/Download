void RenderFrameHostImpl::FilesSelectedInChooser(
    const std::vector<content::FileChooserFileInfo>& files,
    FileChooserParams::Mode permissions) {
  storage::FileSystemContext* const file_system_context =
      BrowserContext::GetStoragePartition(GetProcess()->GetBrowserContext(),
                                          GetSiteInstance())
          ->GetFileSystemContext();
  for (const auto& file : files) {
    if (permissions == FileChooserParams::Save) {
      ChildProcessSecurityPolicyImpl::GetInstance()->GrantCreateReadWriteFile(
          GetProcess()->GetID(), file.file_path);
    } else {
      ChildProcessSecurityPolicyImpl::GetInstance()->GrantReadFile(
          GetProcess()->GetID(), file.file_path);
    }
    if (file.file_system_url.is_valid()) {
      ChildProcessSecurityPolicyImpl::GetInstance()->GrantReadFileSystem(
          GetProcess()->GetID(),
          file_system_context->CrackURL(file.file_system_url)
              .mount_filesystem_id());
    }
  }

  Send(new FrameMsg_RunFileChooserResponse(routing_id_, files));
}
