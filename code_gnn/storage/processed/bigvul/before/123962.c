void RenderViewImpl::didActivateCompositor(int input_handler_identifier) {
#if !defined(OS_MACOSX)   
#if !defined(OS_WIN)   
  InputHandlerManager* input_handler_manager =
      RenderThreadImpl::current()->input_handler_manager();
  if (input_handler_manager)
    input_handler_manager->AddInputHandler(
        routing_id_, input_handler_identifier, AsWeakPtr());
#endif
#endif

  RenderWidget::didActivateCompositor(input_handler_identifier);
}
