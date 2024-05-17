void BrowserPluginGuest::DragStatusUpdate(WebKit::WebDragStatus drag_status,
                                          const WebDropData& drop_data,
                                          WebKit::WebDragOperationsMask mask,
                                          const gfx::Point& location) {
  RenderViewHost* host = web_contents()->GetRenderViewHost();
  switch (drag_status) {
    case WebKit::WebDragStatusEnter:
      host->DragTargetDragEnter(drop_data, location, location, mask, 0);
      break;
    case WebKit::WebDragStatusOver:
      host->DragTargetDragOver(location, location, mask, 0);
      break;
    case WebKit::WebDragStatusLeave:
      host->DragTargetDragLeave();
      break;
    case WebKit::WebDragStatusDrop:
      host->DragTargetDrop(location, location, 0);
      break;
    case WebKit::WebDragStatusUnknown:
      NOTREACHED();
  }
}
