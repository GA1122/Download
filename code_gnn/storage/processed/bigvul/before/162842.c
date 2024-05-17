scoped_refptr<FeatureInfo> GLES2DecoderRestoreStateTest::SetupForES3Test() {
  InitState init;
  init.gl_version = "OpenGL ES 3.0";
  init.context_type = CONTEXT_TYPE_OPENGLES3;
  InitDecoder(init);

  scoped_refptr<FeatureInfo> feature_info = new FeatureInfo;
  TestHelper::SetupFeatureInfoInitExpectationsWithGLVersion(
      gl_.get(), "", "", "OpenGL ES 3.0", CONTEXT_TYPE_OPENGLES3);
  feature_info->InitializeForTesting(CONTEXT_TYPE_OPENGLES3);
  return feature_info;
}
