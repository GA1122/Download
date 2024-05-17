void CheckDuplicateHandle(HANDLE handle) {
  BYTE buffer[sizeof(OBJECT_TYPE_INFORMATION) + 32 * sizeof(wchar_t)];
  OBJECT_TYPE_INFORMATION* type_info =
      reinterpret_cast<OBJECT_TYPE_INFORMATION*>(buffer);
  ULONG size = sizeof(buffer) - sizeof(wchar_t);
  NTSTATUS error;
  error = g_QueryObject(handle, ObjectTypeInformation, type_info, size, &size);
  CHECK(NT_SUCCESS(error));
  type_info->Name.Buffer[type_info->Name.Length / sizeof(wchar_t)] = L'\0';

  OBJECT_BASIC_INFORMATION basic_info;
  size = sizeof(basic_info);
  error = g_QueryObject(handle, ObjectBasicInformation, &basic_info, size,
                        &size);
  CHECK(NT_SUCCESS(error));

  if (0 == _wcsicmp(type_info->Name.Buffer, L"Process")) {
    const ACCESS_MASK kDangerousMask = ~(PROCESS_QUERY_LIMITED_INFORMATION |
                                         SYNCHRONIZE);
    CHECK(!(basic_info.GrantedAccess & kDangerousMask)) <<
        kDuplicateHandleWarning;
  }
}
