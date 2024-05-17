WebGL2RenderingContextBase::WebGL2RenderingContextBase(
    CanvasRenderingContextHost* host,
    std::unique_ptr<WebGraphicsContext3DProvider> context_provider,
    bool using_gpu_compositing,
    const CanvasContextCreationAttributesCore& requested_attributes)
    : WebGLRenderingContextBase(host,
                                std::move(context_provider),
                                using_gpu_compositing,
                                requested_attributes,
                                2) {
  supported_internal_formats_storage_.insert(
      kSupportedInternalFormatsStorage,
      kSupportedInternalFormatsStorage +
          arraysize(kSupportedInternalFormatsStorage));
}
