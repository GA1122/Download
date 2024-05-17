void RenderFrameHostImpl::CreateDedicatedWorkerHostFactory(
    blink::mojom::DedicatedWorkerHostFactoryRequest request) {
  content::CreateDedicatedWorkerHostFactory(process_->GetID(), routing_id_,
                                            last_committed_origin_,
                                            std::move(request));
}
