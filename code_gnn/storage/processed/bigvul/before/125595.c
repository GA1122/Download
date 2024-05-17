void RenderViewHostImpl::DragTargetDragEnter(
    const WebDropData& drop_data,
    const gfx::Point& client_pt,
    const gfx::Point& screen_pt,
    WebDragOperationsMask operations_allowed,
    int key_modifiers) {
  const int renderer_id = GetProcess()->GetID();
  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();

  WebDropData filtered_data(drop_data);
  FilterURL(policy, GetProcess(), true, &filtered_data.url);

  fileapi::IsolatedContext::FileInfoSet files;
  for (std::vector<WebDropData::FileInfo>::iterator iter(
           filtered_data.filenames.begin());
       iter != filtered_data.filenames.end(); ++iter) {
    FilePath path = FilePath::FromUTF8Unsafe(UTF16ToUTF8(iter->path));

    if (iter->display_name.empty()) {
      std::string name;
      files.AddPath(path, &name);
      iter->display_name = UTF8ToUTF16(name);
    } else {
      files.AddPathWithName(path, UTF16ToUTF8(iter->display_name));
    }

    policy->GrantRequestSpecificFileURL(renderer_id,
                                        net::FilePathToFileURL(path));

    if (!policy->CanReadFile(renderer_id, path)) {
      policy->GrantReadFile(renderer_id, path);
      policy->GrantReadDirectory(renderer_id, path);
    }
  }

  fileapi::IsolatedContext* isolated_context =
      fileapi::IsolatedContext::GetInstance();
  DCHECK(isolated_context);
  std::string filesystem_id = isolated_context->RegisterDraggedFileSystem(
      files);
  if (!filesystem_id.empty()) {
    policy->GrantReadFileSystem(renderer_id, filesystem_id);
  }
  filtered_data.filesystem_id = UTF8ToUTF16(filesystem_id);

  Send(new DragMsg_TargetDragEnter(GetRoutingID(), filtered_data, client_pt,
                                   screen_pt, operations_allowed,
                                   key_modifiers));
}
