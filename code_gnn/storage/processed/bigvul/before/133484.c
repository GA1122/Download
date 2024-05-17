int32_t PepperFlashRendererHost::OnIsRectTopmost(
    ppapi::host::HostMessageContext* host_context,
    const PP_Rect& rect) {
  content::PepperPluginInstance* plugin_instance =
      host_->GetPluginInstance(pp_instance());
  if (plugin_instance && plugin_instance->IsRectTopmost(
      gfx::Rect(rect.point.x, rect.point.y,rect.size.width, rect.size.height)))
    return PP_OK;
  return PP_ERROR_FAILED;
}
