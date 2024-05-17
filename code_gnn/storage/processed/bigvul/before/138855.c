TestRenderViewHost::TestRenderViewHost(
    SiteInstance* instance,
    std::unique_ptr<RenderWidgetHostImpl> widget,
    RenderViewHostDelegate* delegate,
    int32_t main_frame_routing_id,
    bool swapped_out)
    : RenderViewHostImpl(instance,
                         std::move(widget),
                         delegate,
                         main_frame_routing_id,
                         swapped_out,
                         false  ),
      delete_counter_(nullptr),
      webkit_preferences_changed_counter_(nullptr),
      opener_frame_route_id_(MSG_ROUTING_NONE) {
  new TestRenderWidgetHostView(GetWidget());
}
