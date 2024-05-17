void OffscreenCanvasSurfaceImpl::GetSurfaceId(
    const GetSurfaceIdCallback& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  surface_id_ = id_allocator_->GenerateId();

   callback.Run(surface_id_);
 }
