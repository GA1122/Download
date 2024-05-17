void GLES2DecoderImpl::FinishAsyncSwapBuffers(
    uint64_t swap_id,
    gfx::SwapResult result,
    std::unique_ptr<gfx::GpuFence> gpu_fence) {
  TRACE_EVENT_ASYNC_END0("gpu", "AsyncSwapBuffers", swap_id);
  DCHECK(!gpu_fence);

  FinishSwapBuffers(result);
}
