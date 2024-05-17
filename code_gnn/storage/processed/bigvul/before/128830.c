void GLSurfaceOzoneSurfaceless::SwapBuffersAsync(
    const SwapCompletionCallback& callback) {
  if (!last_swap_buffers_result_) {
    callback.Run(gfx::SwapResult::SWAP_FAILED);
    return;
  }

  glFlush();

  SwapCompletionCallback surface_swap_callback =
      base::Bind(&GLSurfaceOzoneSurfaceless::SwapCompleted,
                 weak_factory_.GetWeakPtr(), callback);

  PendingFrame* frame = unsubmitted_frames_.back();
  frame->callback = surface_swap_callback;
  unsubmitted_frames_.push_back(new PendingFrame());

  if (has_implicit_external_sync_) {
    EGLSyncKHR fence = InsertFence();
    if (!fence) {
      callback.Run(gfx::SwapResult::SWAP_FAILED);
      return;
    }

    base::Closure fence_wait_task =
        base::Bind(&WaitForFence, GetDisplay(), fence);

    base::Closure fence_retired_callback =
        base::Bind(&GLSurfaceOzoneSurfaceless::FenceRetired,
                   weak_factory_.GetWeakPtr(), fence, frame);

    base::WorkerPool::PostTaskAndReply(FROM_HERE, fence_wait_task,
                                       fence_retired_callback, false);
    return;   
  }

  frame->ready = true;
  SubmitFrame();
}
