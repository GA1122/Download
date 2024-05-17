void GLES2DecoderRestoreStateTest::AddExpectationsForBindSampler(GLuint unit,
                                                                 GLuint id) {
  EXPECT_CALL(*gl_, BindSampler(unit, id)).Times(1).RetiresOnSaturation();
}
