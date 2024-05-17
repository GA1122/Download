bool VaapiWrapper::Initialize(CodecMode mode, VAProfile va_profile) {
  TryToSetVADisplayAttributeToLocalGPU();

  VAEntrypoint entrypoint =
    (mode == kEncode ? VAEntrypointEncSlice : VAEntrypointVLD);
  std::vector<VAConfigAttrib> required_attribs = GetRequiredAttribs(mode);
  base::AutoLock auto_lock(*va_lock_);
  VAStatus va_res = vaCreateConfig(va_display_,
                                   va_profile,
                                   entrypoint,
                                   &required_attribs[0],
                                   required_attribs.size(),
                                   &va_config_id_);
  VA_SUCCESS_OR_RETURN(va_res, "vaCreateConfig failed", false);

  return true;
}
