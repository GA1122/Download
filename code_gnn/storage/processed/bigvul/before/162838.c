void GLES2DecoderRestoreStateTest::AddExpectationsForActiveTexture(
    GLenum unit) {
  EXPECT_CALL(*gl_, ActiveTexture(unit)).Times(1).RetiresOnSaturation();
}
