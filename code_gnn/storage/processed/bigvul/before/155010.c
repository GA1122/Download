WebGLRenderingContextBase::WebGLRenderingContextBase(
    CanvasRenderingContextHost* host,
    std::unique_ptr<WebGraphicsContext3DProvider> context_provider,
    bool using_gpu_compositing,
    const CanvasContextCreationAttributesCore& requested_attributes,
    Platform::ContextType version)
    : WebGLRenderingContextBase(
          host,
          host->GetTopExecutionContext()->GetTaskRunner(TaskType::kWebGL),
          std::move(context_provider),
          using_gpu_compositing,
          requested_attributes,
          version) {}