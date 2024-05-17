void PepperVideoRenderer3D::OnInitialized(int32_t result) {
  CHECK_EQ(result, PP_OK) << "VideoDecoder::Initialize() failed: " << result;
  initialization_finished_ = true;

  DecodeNextPacket();
}
