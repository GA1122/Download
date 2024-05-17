void Document::SetViewportDescription(
    const ViewportDescription& viewport_description) {
  if (viewport_description.IsLegacyViewportType()) {
    if (viewport_description == legacy_viewport_description_)
      return;
    legacy_viewport_description_ = viewport_description;
  } else {
    if (viewport_description == viewport_description_)
      return;
    viewport_description_ = viewport_description;

    if (!viewport_description.IsSpecifiedByAuthor())
      viewport_default_min_width_ = viewport_description.min_width;
  }

  UpdateViewportDescription();
}
