void GLES2Implementation::OnGpuControlSwapBuffersCompleted(
    const SwapBuffersCompleteParams& params) {
  auto found = pending_swap_callbacks_.find(params.swap_response.swap_id);
  if (found == pending_swap_callbacks_.end())
    return;

  std::move(found->second).Run(params);
  pending_swap_callbacks_.erase(found);
}
