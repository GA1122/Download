void ImageResource::DestroyDecodedDataIfPossible() {
  GetContent()->DestroyDecodedData();
  if (GetContent()->HasImage() && !IsUnusedPreload() &&
      GetContent()->IsRefetchableDataFromDiskCache()) {
    UMA_HISTOGRAM_MEMORY_KB("Memory.Renderer.EstimatedDroppableEncodedSize",
                            EncodedSize() / 1024);
  }
}
