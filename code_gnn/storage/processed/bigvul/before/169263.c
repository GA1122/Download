bool IsResizeComplete(aura::test::WindowEventDispatcherTestApi* dispatcher_test,
                      RenderWidgetHostImpl* widget_host) {
  dispatcher_test->WaitUntilPointerMovesDispatched();
  widget_host->SynchronizeVisualProperties();
  return !widget_host->resize_ack_pending_for_testing();
}
