  MockRenderWidgetHostImpl(RenderWidgetHostDelegate* delegate,
                           RenderProcessHost* process,
                           int32_t routing_id,
                           std::unique_ptr<MockWidgetImpl> widget_impl,
                           mojom::WidgetPtr widget)
      : RenderWidgetHostImpl(delegate,
                             process,
                             routing_id,
                             std::move(widget),
                             false),
        widget_impl_(std::move(widget_impl)) {
    lastWheelOrTouchEventLatencyInfo = ui::LatencyInfo();
  }
