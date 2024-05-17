IntSize LocalDOMWindow::GetViewportSize() const {
  LocalFrameView* view = GetFrame()->View();
  if (!view)
    return IntSize();

  Page* page = GetFrame()->GetPage();
  if (!page)
    return IntSize();

  if (page->GetSettings().GetViewportEnabled() && GetFrame()->IsMainFrame())
    document()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  if (Frame* parent = GetFrame()->Tree().Parent()) {
    if (parent && parent->IsLocalFrame())
      ToLocalFrame(parent)
          ->GetDocument()
          ->UpdateStyleAndLayoutIgnorePendingStylesheets();
  }

  return document()->View()->Size();
}
