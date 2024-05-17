Element* LocalDOMWindow::frameElement() const {
  if (!(GetFrame() && GetFrame()->Owner() && GetFrame()->Owner()->IsLocal()))
    return nullptr;

  return ToHTMLFrameOwnerElement(GetFrame()->Owner());
}
