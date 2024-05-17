void GLES2DecoderRestoreStateTest::AddExpectationsForBindTexture(GLenum target,
                                                                 GLuint id) {
  EXPECT_CALL(*gl_, BindTexture(target, id)).Times(1).RetiresOnSaturation();
}
