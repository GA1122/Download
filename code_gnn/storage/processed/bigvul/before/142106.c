HRESULT RegisterWithGoogleDeviceManagement(const base::string16& mdm_url,
                                           const base::string16& email,
                                           const std::string& data) {
  switch (g_enrollment_status) {
    case EnrollmentStatus::kForceSuccess:
      return S_OK;
    case EnrollmentStatus::kForceFailure:
      return E_FAIL;
    case EnrollmentStatus::kDontForce:
      break;
  }

  if (IsEnrolledWithGoogleMdm(mdm_url)) {
    LOGFN(INFO) << "Already enrolled to Google MDM";
    return S_OK;
  }

  base::ScopedNativeLibrary library(
      base::FilePath(FILE_PATH_LITERAL("MDMRegistration.dll")));
  auto register_device_with_management_function =
      GET_MDM_FUNCTION_POINTER(library, RegisterDeviceWithManagement);
  if (!register_device_with_management_function) {
    LOGFN(ERROR) << "GET_MDM_FUNCTION_POINTER(RegisterDeviceWithManagement)";
    return false;
  }

  std::string data_encoded;
  base::Base64Encode(data, &data_encoded);

  return register_device_with_management_function(
      email.c_str(), mdm_url.c_str(), base::UTF8ToWide(data_encoded).c_str());
}
