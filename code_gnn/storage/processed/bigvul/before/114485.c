void EglRenderingVDAClient::ProvidePictureBuffers(
    uint32 requested_num_of_buffers,
    const gfx::Size& dimensions) {
  if (decoder_deleted())
    return;
  std::vector<media::PictureBuffer> buffers;

  for (uint32 i = 0; i < requested_num_of_buffers; ++i) {
    uint32 id = picture_buffers_by_id_.size();
    GLuint texture_id;
    base::WaitableEvent done(false, false);
    rendering_helper_->CreateTexture(rendering_window_id_, &texture_id, &done);
    done.Wait();
    CHECK(outstanding_texture_ids_.insert(texture_id).second);
    media::PictureBuffer* buffer =
        new media::PictureBuffer(id, dimensions, texture_id);
    CHECK(picture_buffers_by_id_.insert(std::make_pair(id, buffer)).second);
    buffers.push_back(*buffer);
  }
  decoder_->AssignPictureBuffers(buffers);
  CHECK_EQ(static_cast<int>(glGetError()), GL_NO_ERROR);
  CHECK_EQ(static_cast<int>(eglGetError()), EGL_SUCCESS);
}
