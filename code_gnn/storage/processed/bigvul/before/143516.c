void CompositorImpl::OnFatalOrSurfaceContextCreationFailure(
    gpu::ContextResult context_result) {
  DCHECK(gpu::IsFatalOrSurfaceFailure(context_result));
  LOG_IF(FATAL, context_result == gpu::ContextResult::kFatalFailure)
      << "Fatal error making Gpu context";

  if (context_result == gpu::ContextResult::kSurfaceFailure) {
    SetSurface(nullptr);
    client_->RecreateSurface();
  }
}
