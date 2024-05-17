void TestRenderViewHost::TestOnStartDragging(
    const DropData& drop_data) {
  blink::WebDragOperationsMask drag_operation = blink::kWebDragOperationEvery;
  DragEventSourceInfo event_info;
  GetWidget()->OnStartDragging(drop_data, drag_operation, SkBitmap(),
                               gfx::Vector2d(), event_info);
}
