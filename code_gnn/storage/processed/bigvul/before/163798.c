void DelegatedFrameHost::WillDrawSurface(const viz::LocalSurfaceId& id,
                                         const gfx::Rect& damage_rect) {
  if (id != local_surface_id_)
    return;
  AttemptFrameSubscriberCapture(damage_rect);
}
