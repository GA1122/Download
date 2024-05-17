Error* Automation::CheckVersion(int client_build_no,
                                int client_patch_no,
                                const std::string& error_msg) {
  bool version_is_ok = false;
  Error* error = CompareVersion(
      client_build_no, client_patch_no, &version_is_ok);
  if (error)
    return error;
  if (!version_is_ok)
    return new Error(kUnknownError, error_msg);
  return NULL;
}
