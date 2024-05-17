void CreateMemoryCoordinatorHandle(
    int render_process_id,
    mojom::MemoryCoordinatorHandleRequest request) {
  MemoryCoordinatorImpl::GetInstance()->CreateHandle(render_process_id,
                                                     std::move(request));
}
