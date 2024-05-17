bool VaapiWrapper::BlitSurface(VASurfaceID va_surface_id_src,
                               const gfx::Size& src_size,
                               VASurfaceID va_surface_id_dest,
                               const gfx::Size& dest_size) {
  base::AutoLock auto_lock(*va_lock_);

  if (va_vpp_buffer_id_ == VA_INVALID_ID) {
    if (!InitializeVpp_Locked())
      return false;
  }

  VAProcPipelineParameterBuffer* pipeline_param;
  VA_SUCCESS_OR_RETURN(vaMapBuffer(va_display_, va_vpp_buffer_id_,
                                   reinterpret_cast<void**>(&pipeline_param)),
                       "Couldn't map vpp buffer", false);

  memset(pipeline_param, 0, sizeof *pipeline_param);

  VARectangle input_region;
  input_region.x = input_region.y = 0;
  input_region.width = src_size.width();
  input_region.height = src_size.height();
  pipeline_param->surface_region = &input_region;
  pipeline_param->surface = va_surface_id_src;
  pipeline_param->surface_color_standard = VAProcColorStandardNone;

  VARectangle output_region;
  output_region.x = output_region.y = 0;
  output_region.width = dest_size.width();
  output_region.height = dest_size.height();
  pipeline_param->output_region = &output_region;
  pipeline_param->output_background_color = 0xff000000;
  pipeline_param->output_color_standard = VAProcColorStandardNone;

  VA_SUCCESS_OR_RETURN(vaUnmapBuffer(va_display_, va_vpp_buffer_id_),
                       "Couldn't unmap vpp buffer", false);

  VA_SUCCESS_OR_RETURN(
      vaBeginPicture(va_display_, va_vpp_context_id_, va_surface_id_dest),
      "Couldn't begin picture", false);

  VA_SUCCESS_OR_RETURN(
      vaRenderPicture(va_display_, va_vpp_context_id_, &va_vpp_buffer_id_, 1),
      "Couldn't render picture", false);

  VA_SUCCESS_OR_RETURN(vaEndPicture(va_display_, va_vpp_context_id_),
                       "Couldn't end picture", false);

  return true;
}
