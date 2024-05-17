bool RenderView::IsEditableNode(const WebNode& node) {
  bool is_editable_node = false;
  if (!node.isNull()) {
    if (node.isContentEditable()) {
      is_editable_node = true;
    } else if (node.isElementNode()) {
      is_editable_node =
          node.toConst<WebElement>().isTextFormControlElement();
    }
  }
  return is_editable_node;
}
