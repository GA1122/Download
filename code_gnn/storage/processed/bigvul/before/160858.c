static void DragAndDropURL(WebViewImpl* web_view, const std::string& url) {
  WebDragData drag_data;
  drag_data.Initialize();

  WebDragData::Item item;
  item.storage_type = WebDragData::Item::kStorageTypeString;
  item.string_type = "text/uri-list";
  item.string_data = WebString::FromUTF8(url);
  drag_data.AddItem(item);

  const WebFloatPoint client_point(0, 0);
  const WebFloatPoint screen_point(0, 0);
  WebFrameWidgetBase* widget = web_view->MainFrameImpl()->FrameWidget();
  widget->DragTargetDragEnter(drag_data, client_point, screen_point,
                              kWebDragOperationCopy, 0);
  widget->DragTargetDrop(drag_data, client_point, screen_point, 0);
  FrameTestHelpers::PumpPendingRequestsForFrameToLoad(web_view->MainFrame());
}
