void RenderWidgetHostViewAura::CopyFromCompositingSurfaceFinishedForVideo(
    base::WeakPtr<RenderWidgetHostViewAura> rwhva,
    const base::Callback<void(bool)>& callback,
    scoped_refptr<OwnedMailbox> subscriber_texture,
    scoped_ptr<cc::SingleReleaseCallback> release_callback,
    bool result) {
  callback.Run(result);

  GLHelper* gl_helper = ImageTransportFactory::GetInstance()->GetGLHelper();
  uint32 sync_point = gl_helper ? gl_helper->InsertSyncPoint() : 0;
  if (release_callback) {
    DCHECK(!subscriber_texture);
    release_callback->Run(sync_point, false);
  }
  ReturnSubscriberTexture(rwhva, subscriber_texture, sync_point);
}
