void ImageBitmapFactories::ImageBitmapLoader::DidFail(FileErrorCode) {
  RejectPromise(kUndecodableImageBitmapRejectionReason);
}
