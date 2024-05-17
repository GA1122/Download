scoped_refptr<DrawingBuffer> WebGLRenderingContextBase::CreateDrawingBuffer(
    std::unique_ptr<WebGraphicsContext3DProvider> context_provider,
    bool using_gpu_compositing) {
  bool premultiplied_alpha = CreationAttributes().premultiplied_alpha;
  bool want_alpha_channel = CreationAttributes().alpha;
  bool want_depth_buffer = CreationAttributes().depth;
  bool want_stencil_buffer = CreationAttributes().stencil;
  bool want_antialiasing = CreationAttributes().antialias;
  DrawingBuffer::PreserveDrawingBuffer preserve =
      CreationAttributes().preserve_drawing_buffer ? DrawingBuffer::kPreserve
                                                   : DrawingBuffer::kDiscard;
  DrawingBuffer::WebGLVersion web_gl_version = DrawingBuffer::kWebGL1;
  if (Version() == 1) {
    web_gl_version = DrawingBuffer::kWebGL1;
  } else if (Version() == 2) {
    web_gl_version = DrawingBuffer::kWebGL2;
  } else {
    NOTREACHED();
  }

  DrawingBuffer::ChromiumImageUsage chromium_image_usage =
      Host()->IsOffscreenCanvas() ? DrawingBuffer::kDisallowChromiumImage
                                  : DrawingBuffer::kAllowChromiumImage;

  return DrawingBuffer::Create(
      std::move(context_provider), using_gpu_compositing, this,
      ClampedCanvasSize(), premultiplied_alpha, want_alpha_channel,
      want_depth_buffer, want_stencil_buffer, want_antialiasing, preserve,
      web_gl_version, chromium_image_usage, ColorParams());
}
