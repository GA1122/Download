blink::WebDragOperationsMask ConvertToWeb(int drag_op) {
  int web_drag_op = blink::WebDragOperationNone;
  if (drag_op & ui::DragDropTypes::DRAG_COPY)
    web_drag_op |= blink::WebDragOperationCopy;
  if (drag_op & ui::DragDropTypes::DRAG_MOVE)
    web_drag_op |= blink::WebDragOperationMove;
  if (drag_op & ui::DragDropTypes::DRAG_LINK)
    web_drag_op |= blink::WebDragOperationLink;
  return (blink::WebDragOperationsMask) web_drag_op;
}
