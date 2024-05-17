void PepperVideoRenderer3D::GetNextPicture() {
  if (get_picture_pending_)
    return;

  int32_t result =
      video_decoder_.GetPicture(callback_factory_.NewCallbackWithOutput(
          &PepperVideoRenderer3D::OnPictureReady));
  CHECK_EQ(result, PP_OK_COMPLETIONPENDING);
  get_picture_pending_ = true;
}
