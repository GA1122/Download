bool FileSystemPolicy::SetInformationFileAction(
    EvalResult eval_result, const ClientInfo& client_info,
    HANDLE target_file_handle, void* file_info, uint32 length,
    uint32 info_class, IO_STATUS_BLOCK* io_block,
    NTSTATUS* nt_status) {
  if (ASK_BROKER != eval_result) {
    *nt_status = STATUS_ACCESS_DENIED;
    return true;
  }

  NtSetInformationFileFunction NtSetInformationFile = NULL;
  ResolveNTFunctionPtr("NtSetInformationFile", &NtSetInformationFile);

  HANDLE local_handle = NULL;
  if (!::DuplicateHandle(client_info.process, target_file_handle,
                         ::GetCurrentProcess(), &local_handle, 0, FALSE,
                         DUPLICATE_SAME_ACCESS)) {
    *nt_status = STATUS_ACCESS_DENIED;
    return true;
  }

  ScopedHandle handle(local_handle);

  FILE_INFORMATION_CLASS file_info_class =
      static_cast<FILE_INFORMATION_CLASS>(info_class);
  *nt_status = NtSetInformationFile(local_handle, io_block, file_info, length,
                                    file_info_class);

  return true;
}
