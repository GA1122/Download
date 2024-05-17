void WebMediaPlayerMS::EnterPictureInPicture(
    blink::WebMediaPlayer::PipWindowOpenedCallback callback) {
  DCHECK(bridge_);

  const viz::SurfaceId& surface_id = bridge_->GetSurfaceId();
  DCHECK(surface_id.is_valid());

  delegate_->DidPictureInPictureModeStart(delegate_id_, surface_id,
                                          NaturalSize(), std::move(callback),
                                          false  );
}
