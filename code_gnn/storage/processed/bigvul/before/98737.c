void WebPluginDelegatePepper::ScheduleHandleRepaint(
    NPP npp, NPDeviceContext3D* context) {
  command_buffer_->SetNotifyRepaintTask(method_factory3d_.NewRunnableMethod(
      &WebPluginDelegatePepper::ForwardHandleRepaint,
      npp,
      context));
}
