bool VaapiWrapper::IsEntrypointSupported_Locked(VAProfile va_profile,
                                                VAEntrypoint entrypoint) {
  va_lock_->AssertAcquired();
  int max_entrypoints = vaMaxNumEntrypoints(va_display_);
  std::vector<VAEntrypoint> supported_entrypoints(
      base::checked_cast<size_t>(max_entrypoints));

  int num_supported_entrypoints;
  VAStatus va_res = vaQueryConfigEntrypoints(va_display_,
                                             va_profile,
                                             &supported_entrypoints[0],
                                             &num_supported_entrypoints);
  VA_SUCCESS_OR_RETURN(va_res, "vaQueryConfigEntrypoints failed", false);
  if (num_supported_entrypoints < 0 ||
      num_supported_entrypoints > max_entrypoints) {
    LOG(ERROR) << "vaQueryConfigEntrypoints returned: "
               << num_supported_entrypoints;
    return false;
  }

  if (std::find(supported_entrypoints.begin(),
                supported_entrypoints.end(),
                entrypoint) == supported_entrypoints.end()) {
    DVLOG(1) << "Unsupported entrypoint";
    return false;
  }
  return true;
}
