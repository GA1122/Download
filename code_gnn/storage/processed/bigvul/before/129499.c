void GLES2DecoderImpl::SetAsyncPixelTransferManagerForTest(
    AsyncPixelTransferManager* manager) {
  async_pixel_transfer_manager_ = make_scoped_ptr(manager);
}
