void VaapiVideoDecodeAccelerator::TryFinishSurfaceSetChange() {
  DCHECK(task_runner_->BelongsToCurrentThread());

  if (!awaiting_va_surfaces_recycle_)
    return;

  if (!pending_output_cbs_.empty() ||
      pictures_.size() != available_va_surfaces_.size()) {
    DVLOGF(2) << "Awaiting pending output/surface release callbacks to finish";
    task_runner_->PostTask(
        FROM_HERE,
        base::Bind(&VaapiVideoDecodeAccelerator::TryFinishSurfaceSetChange,
                   weak_this_));
    return;
  }

  awaiting_va_surfaces_recycle_ = false;
  available_va_surfaces_.clear();
  vaapi_wrapper_->DestroySurfaces();

  for (Pictures::iterator iter = pictures_.begin(); iter != pictures_.end();
       ++iter) {
    VLOGF(2) << "Dismissing picture id: " << iter->first;
    if (client_)
      client_->DismissPictureBuffer(iter->first);
  }
  pictures_.clear();

  VLOGF(2) << "Requesting " << requested_num_pics_
           << " pictures of size: " << requested_pic_size_.ToString();

  VideoPixelFormat format = GfxBufferFormatToVideoPixelFormat(output_format_);
  task_runner_->PostTask(
      FROM_HERE, base::Bind(&Client::ProvidePictureBuffers, client_,
                            requested_num_pics_, format, 1, requested_pic_size_,
                            vaapi_picture_factory_->GetGLTextureTarget()));
}
