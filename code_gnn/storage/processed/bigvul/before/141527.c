bool PaintLayerScrollableArea::VisualViewportSuppliesScrollbars() const {
  LocalFrame* frame = GetLayoutBox()->GetFrame();
  if (!frame || !frame->GetSettings())
    return false;

  if (!frame->GetSettings()->GetViewportEnabled())
    return false;

  const TopDocumentRootScrollerController& controller =
      GetLayoutBox()->GetDocument().GetPage()->GlobalRootScrollerController();
  return controller.RootScrollerArea() == this;
}
