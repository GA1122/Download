DWORD SetObjectIntegrityLabel(HANDLE handle, SE_OBJECT_TYPE type,
                              const wchar_t* ace_access,
                              const wchar_t* integrity_level_sid) {
  std::wstring sddl = L"S:(";      
  sddl += SDDL_MANDATORY_LABEL;    
  sddl += L";;";                   
  sddl += ace_access;              
  sddl += L";;;";                  
  sddl += integrity_level_sid;     
  sddl += L")";

  DWORD error = ERROR_SUCCESS;
  PSECURITY_DESCRIPTOR sec_desc = NULL;

  PACL sacl = NULL;
  BOOL sacl_present = FALSE;
  BOOL sacl_defaulted = FALSE;

  if (::ConvertStringSecurityDescriptorToSecurityDescriptorW(sddl.c_str(),
                                                             SDDL_REVISION,
                                                             &sec_desc, NULL)) {
    if (::GetSecurityDescriptorSacl(sec_desc, &sacl_present, &sacl,
                                    &sacl_defaulted)) {
      error = ::SetSecurityInfo(handle, type,
                                LABEL_SECURITY_INFORMATION, NULL, NULL, NULL,
                                sacl);
    } else {
      error = ::GetLastError();
    }

    ::LocalFree(sec_desc);
  } else {
    return::GetLastError();
  }

  return error;
}
