bool FileSystemPolicy::CreateFileAction(EvalResult eval_result,
                                        const ClientInfo& client_info,
                                        const std::wstring &file,
                                        uint32 attributes,
                                        uint32 desired_access,
                                        uint32 file_attributes,
                                        uint32 share_access,
                                        uint32 create_disposition,
                                        uint32 create_options,
                                        HANDLE *handle,
                                        NTSTATUS* nt_status,
                                        ULONG_PTR *io_information) {
  if (ASK_BROKER != eval_result) {
    *nt_status = STATUS_ACCESS_DENIED;
    return false;
  }
  IO_STATUS_BLOCK io_block = {0};
  UNICODE_STRING uni_name = {0};
  OBJECT_ATTRIBUTES obj_attributes = {0};
  InitObjectAttribs(file, attributes, NULL, &obj_attributes, &uni_name);
  *nt_status = NtCreateFileInTarget(handle, desired_access, &obj_attributes,
                                    &io_block, file_attributes, share_access,
                                    create_disposition, create_options, NULL,
                                    0, client_info.process);

  *io_information = io_block.Information;
  return true;
}
