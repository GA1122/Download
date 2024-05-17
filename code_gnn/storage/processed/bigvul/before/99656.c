void VaapiVideoDecodeAccelerator::TryFinishSurfaceSetChange() {
  DCHECK_EQ(message_loop_, base::MessageLoop::current());

  if (!awaiting_va_surfaces_recycle_)
    return;

  if (!pending_output_cbs_.empty() ||
      pictures_.size() != available_va_surfaces_.size()) {
    DVLOG(2) << "Awaiting pending output/surface release callbacks to finish";
    message_loop_->PostTask(FROM_HERE, base::Bind(
        &VaapiVideoDecodeAccelerator::TryFinishSurfaceSetChange, weak_this_));
    return;
  }

  awaiting_va_surfaces_recycle_ = false;
  available_va_surfaces_.clear();
  vaapi_wrapper_->DestroySurfaces();

  for (Pictures::iterator iter = pictures_.begin(); iter != pictures_.end();
       ++iter) {
    DVLOG(2) << "Dismissing picture id: " << iter->first;
    if (client_)
      client_->DismissPictureBuffer(iter->first);
  }
  pictures_.clear();

  DVLOG(1) << "Requesting " << requested_num_pics_ << " pictures of size: "
           << requested_pic_size_.ToString();

  message_loop_->PostTask(
      FROM_HERE,
      base::Bind(&Client::ProvidePictureBuffers, client_, requested_num_pics_,
                 requested_pic_size_, VaapiPicture::GetGLTextureTarget()));
}
