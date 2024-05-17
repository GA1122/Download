bool IsEnrolledWithGoogleMdm(const base::string16& mdm_url) {
  switch (g_enrolled_status) {
    case EnrolledStatus::kForceTrue:
      return true;
    case EnrolledStatus::kForceFalse:
      return false;
    case EnrolledStatus::kDontForce:
      break;
  }

  base::ScopedNativeLibrary library(
      base::FilePath(FILE_PATH_LITERAL("MDMRegistration.dll")));
  auto get_device_registration_info_function =
      GET_MDM_FUNCTION_POINTER(library, GetDeviceRegistrationInfo);
  if (!get_device_registration_info_function) {
    LOGFN(ERROR) << "GET_MDM_FUNCTION_POINTER(GetDeviceRegistrationInfo)";
    return false;
  }

  MANAGEMENT_REGISTRATION_INFO* info;
  HRESULT hr = get_device_registration_info_function(
      DeviceRegistrationBasicInfo, reinterpret_cast<void**>(&info));

  bool is_enrolled = SUCCEEDED(hr) && info->fDeviceRegisteredWithManagement &&
                     GURL(mdm_url) == GURL(info->pszMDMServiceUri);

  if (SUCCEEDED(hr))
    ::HeapFree(::GetProcessHeap(), 0, info);
  return is_enrolled;
}
