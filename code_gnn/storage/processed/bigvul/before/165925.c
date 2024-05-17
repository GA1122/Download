void RenderFrameImpl::PepperCancelComposition(
    PepperPluginInstanceImpl* instance) {
  if (instance != focused_pepper_plugin_)
    return;
  if (mojom::WidgetInputHandlerHost* host = GetRenderWidget()
                                                ->widget_input_handler_manager()
                                                ->GetWidgetInputHandlerHost()) {
    host->ImeCancelComposition();
  }
#if defined(OS_MACOSX) || defined(USE_AURA)
  GetRenderWidget()->UpdateCompositionInfo(
      false  );
#endif
}
