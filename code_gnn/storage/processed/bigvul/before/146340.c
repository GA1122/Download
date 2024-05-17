ImageData* WebGLRenderingContextBase::PaintRenderingResultsToImageData(
    SourceDrawingBuffer source_buffer) {
  if (isContextLost())
    return nullptr;
  if (CreationAttributes().premultipliedAlpha())
    return nullptr;

  ClearIfComposited();
  GetDrawingBuffer()->ResolveAndBindForReadAndDraw();
  ScopedFramebufferRestorer restorer(this);
  int width, height;
  WTF::ArrayBufferContents contents;
  if (!GetDrawingBuffer()->PaintRenderingResultsToImageData(
          width, height, source_buffer, contents))
    return nullptr;
  DOMArrayBuffer* image_data_pixels = DOMArrayBuffer::Create(contents);

  return ImageData::Create(
      IntSize(width, height),
      NotShared<DOMUint8ClampedArray>(DOMUint8ClampedArray::Create(
          image_data_pixels, 0, image_data_pixels->ByteLength())));
}
