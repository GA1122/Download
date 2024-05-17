void Dispatcher::InvokeModuleSystemMethod(content::RenderFrame* render_frame,
                                          const std::string& extension_id,
                                          const std::string& module_name,
                                          const std::string& function_name,
                                          const base::ListValue& args,
                                          bool user_gesture) {
  scoped_ptr<WebScopedUserGesture> web_user_gesture;
  if (user_gesture)
    web_user_gesture.reset(new WebScopedUserGesture);

  script_context_set_->ForEach(
      extension_id, render_frame,
      base::Bind(&CallModuleMethod, module_name, function_name, &args));

  if (set_idle_notifications_) {
    RenderThread::Get()->ScheduleIdleHandler(
        kInitialExtensionIdleHandlerDelayMs);
  }

  const Extension* extension =
      RendererExtensionRegistry::Get()->GetByID(extension_id);
  if (extension && BackgroundInfo::HasLazyBackgroundPage(extension) &&
      module_name == kEventBindings &&
      function_name == kEventDispatchFunction) {
    content::RenderFrame* background_frame =
        ExtensionFrameHelper::GetBackgroundPageFrame(extension_id);
    if (background_frame) {
      int message_id;
      args.GetInteger(3, &message_id);
      background_frame->Send(new ExtensionHostMsg_EventAck(
          background_frame->GetRoutingID(), message_id));
    }
  }
}
