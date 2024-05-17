VaapiPicture* VaapiVideoDecodeAccelerator::PictureById(
    int32 picture_buffer_id) {
  Pictures::iterator it = pictures_.find(picture_buffer_id);
  if (it == pictures_.end()) {
    LOG(ERROR) << "Picture id " << picture_buffer_id << " does not exist";
    return NULL;
  }

  return it->second.get();
}
