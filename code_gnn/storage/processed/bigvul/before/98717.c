void WebPluginDelegatePepper::ForwardHandleRepaint(
    NPP npp, NPDeviceContext3D* context) {
  if (context->repaintCallback)
    context->repaintCallback(npp, context);
  ScheduleHandleRepaint(npp, context);
}
