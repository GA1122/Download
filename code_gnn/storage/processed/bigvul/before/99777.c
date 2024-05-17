bool FileSystemPolicy::QueryFullAttributesFileAction(
    EvalResult eval_result,
    const ClientInfo& client_info,
    const std::wstring &file,
    uint32 attributes,
    FILE_NETWORK_OPEN_INFORMATION* file_info,
    NTSTATUS* nt_status) {
  if (ASK_BROKER != eval_result) {
    *nt_status = STATUS_ACCESS_DENIED;
    return true;
  }

  NtQueryFullAttributesFileFunction NtQueryFullAttributesFile = NULL;
  ResolveNTFunctionPtr("NtQueryFullAttributesFile", &NtQueryFullAttributesFile);

  UNICODE_STRING uni_name = {0};
  OBJECT_ATTRIBUTES obj_attributes = {0};
  InitObjectAttribs(file, attributes, NULL, &obj_attributes, &uni_name);
  *nt_status = NtQueryFullAttributesFile(&obj_attributes, file_info);

  return true;
}
