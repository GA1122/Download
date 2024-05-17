static Image* GetImage(Element* element) {
  DCHECK(element);
  ImageResourceContent* cached_image = GetImageResource(element);
  return (cached_image && !cached_image->ErrorOccurred())
             ? cached_image->GetImage()
             : nullptr;
}
