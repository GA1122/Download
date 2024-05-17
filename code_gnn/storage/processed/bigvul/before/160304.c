LayoutImageResource* ImageLoader::GetLayoutImageResource() {
  LayoutObject* layout_object = element_->GetLayoutObject();

  if (!layout_object)
    return nullptr;

  if (layout_object->IsImage() &&
      !ToLayoutImage(layout_object)->IsGeneratedContent())
    return ToLayoutImage(layout_object)->ImageResource();

  if (layout_object->IsSVGImage())
    return ToLayoutSVGImage(layout_object)->ImageResource();

  if (layout_object->IsVideo())
    return ToLayoutVideo(layout_object)->ImageResource();

  return nullptr;
}
