bool ChromeContentClient::GetSandboxProfileForSandboxType(
    int sandbox_type,
    int* sandbox_profile_resource_id) const {
  DCHECK(sandbox_profile_resource_id);
  if (sandbox_type == CHROME_SANDBOX_TYPE_NACL_LOADER) {
    *sandbox_profile_resource_id = IDR_NACL_SANDBOX_PROFILE;
    return true;
  }
  return false;
}
