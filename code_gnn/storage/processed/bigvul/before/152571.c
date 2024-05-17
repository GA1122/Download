  void CloseRenderWidget(RenderWidget* widget) {
    WidgetMsg_Close msg(widget->routing_id());
    widget->OnMessageReceived(msg);

    base::RunLoop().RunUntilIdle();
  }
