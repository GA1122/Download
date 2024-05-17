bool VaapiWrapper::GetMaxResolution_Locked(
    VAProfile va_profile,
    VAEntrypoint entrypoint,
    std::vector<VAConfigAttrib>& required_attribs,
    gfx::Size* resolution) {
  va_lock_->AssertAcquired();
  VAConfigID va_config_id;
  VAStatus va_res = vaCreateConfig(
      va_display_,
      va_profile,
      entrypoint,
      &required_attribs[0],
      required_attribs.size(),
      &va_config_id);
  VA_SUCCESS_OR_RETURN(va_res, "vaCreateConfig failed", false);

  unsigned int num_attribs;
  va_res = vaQuerySurfaceAttributes(
      va_display_, va_config_id, nullptr, &num_attribs);
  VA_SUCCESS_OR_RETURN(va_res, "vaQuerySurfaceAttributes failed", false);
  if (!num_attribs)
    return false;

  std::vector<VASurfaceAttrib> attrib_list(
      base::checked_cast<size_t>(num_attribs));

  va_res = vaQuerySurfaceAttributes(
      va_display_, va_config_id, &attrib_list[0], &num_attribs);
  VA_SUCCESS_OR_RETURN(va_res, "vaQuerySurfaceAttributes failed", false);

  resolution->SetSize(0, 0);
  for (const auto& attrib : attrib_list) {
    if (attrib.type == VASurfaceAttribMaxWidth)
      resolution->set_width(attrib.value.value.i);
    else if (attrib.type == VASurfaceAttribMaxHeight)
      resolution->set_height(attrib.value.value.i);
  }
  if (resolution->IsEmpty()) {
    LOG(ERROR) << "Codec resolution " << resolution->ToString()
               << " cannot be zero.";
    return false;
  }
  return true;
}
