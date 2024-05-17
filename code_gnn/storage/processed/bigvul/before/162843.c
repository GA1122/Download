void SetupUpdateES3UnpackParametersExpectations(::gl::MockGLInterface* gl,
                                                GLint row_length,
                                                GLint image_height) {
  EXPECT_CALL(*gl, PixelStorei(GL_UNPACK_ROW_LENGTH, row_length))
      .Times(1)
      .RetiresOnSaturation();
  EXPECT_CALL(*gl, PixelStorei(GL_UNPACK_IMAGE_HEIGHT, image_height))
      .Times(1)
      .RetiresOnSaturation();
}
