scoped_refptr<StreamTextureFactory> RenderThreadImpl::GetStreamTexureFactory() {
  DCHECK(IsMainThread());
  if (!stream_texture_factory_.get() ||
      stream_texture_factory_->ContextGL()->GetGraphicsResetStatusKHR() !=
          GL_NO_ERROR) {
    scoped_refptr<ui::ContextProviderCommandBuffer> shared_context_provider =
        SharedMainThreadContextProvider();
    if (!shared_context_provider) {
      stream_texture_factory_ = nullptr;
      return nullptr;
    }
    DCHECK(shared_context_provider->GetCommandBufferProxy());
    DCHECK(shared_context_provider->GetCommandBufferProxy()->channel());
    stream_texture_factory_ =
        StreamTextureFactory::Create(std::move(shared_context_provider));
  }
  return stream_texture_factory_;
}
