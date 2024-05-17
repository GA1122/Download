void WebMediaPlayerImpl::OnSurfaceIdUpdated(viz::SurfaceId surface_id) {
  if (client_ && IsInPictureInPicture() && !client_->IsInAutoPIP()) {
    delegate_->DidPictureInPictureSurfaceChange(
        delegate_id_, surface_id, pipeline_metadata_.natural_size,
        true  );
  }
}
