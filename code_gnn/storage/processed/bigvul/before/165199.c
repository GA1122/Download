static ImageResourceContent* GetImageResource(Element* element) {
  DCHECK(element);
  LayoutObject* layout_object = element->GetLayoutObject();
  if (!layout_object || !layout_object->IsImage())
    return nullptr;
  LayoutImage* image = ToLayoutImage(layout_object);
  return image->CachedImage();
}
