void GLSurfaceEGLSurfaceControl::OnTransactionAckOnGpuThread(
    SwapCompletionCallback completion_callback,
    PresentationCallback presentation_callback,
    ResourceRefs released_resources,
    SurfaceControl::TransactionStats transaction_stats) {
  DCHECK(gpu_task_runner_->BelongsToCurrentThread());
  context_->MakeCurrent(this);

  std::move(completion_callback).Run(gfx::SwapResult::SWAP_ACK, nullptr);

  gfx::PresentationFeedback feedback(base::TimeTicks::Now(), base::TimeDelta(),
                                     0  );
  std::move(presentation_callback).Run(feedback);

  for (auto& surface_stat : transaction_stats.surface_stats) {
    auto it = released_resources.find(surface_stat.surface);
    DCHECK(it != released_resources.end());
    if (surface_stat.fence.is_valid())
      it->second.scoped_buffer->SetReadFence(std::move(surface_stat.fence));
  }
  released_resources.clear();
}
