static PassRefPtr<Image> ImageFromNode(const Node& node) {
  DCHECK(!node.GetDocument().NeedsLayoutTreeUpdate());
  DocumentLifecycle::DisallowTransitionScope disallow_transition(
      node.GetDocument().Lifecycle());

  LayoutObject* layout_object = node.GetLayoutObject();
  if (!layout_object)
    return nullptr;

  if (layout_object->IsCanvas()) {
    return toHTMLCanvasElement(const_cast<Node&>(node))
        .CopiedImage(kFrontBuffer, kPreferNoAcceleration,
                     kSnapshotReasonCopyToClipboard);
  }

  if (layout_object->IsImage()) {
    LayoutImage* layout_image = ToLayoutImage(layout_object);
    if (!layout_image)
      return nullptr;

    ImageResourceContent* cached_image = layout_image->CachedImage();
    if (!cached_image || cached_image->ErrorOccurred())
      return nullptr;
    return cached_image->GetImage();
  }

  return nullptr;
}
