void RenderProcessHostImpl::CreateMusGpuRequest(ui::mojom::GpuRequest request) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!gpu_client_)
    gpu_client_.reset(new GpuClient(GetID()));
  gpu_client_->Add(std::move(request));
}
