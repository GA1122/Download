WebGLRenderingContextBase::WebGLRenderingContextBase(
    CanvasRenderingContextHost* host,
    std::unique_ptr<WebGraphicsContext3DProvider> context_provider,
    const CanvasContextCreationAttributes& requested_attributes,
    unsigned version)
    : WebGLRenderingContextBase(
          host,
          TaskRunnerHelper::Get(TaskType::kWebGL,
                                host->GetTopExecutionContext()),
          std::move(context_provider),
          requested_attributes,
          version) {}
