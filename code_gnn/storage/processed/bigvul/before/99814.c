void WebPluginDelegateStub::OnUpdateGeometry(
    const PluginMsg_UpdateGeometry_Param& param) {
  webplugin_->UpdateGeometry(
      param.window_rect, param.clip_rect,
      param.windowless_buffer, param.background_buffer
#if defined(OS_MACOSX)
      ,
      param.ack_key
#endif
       );
 }
