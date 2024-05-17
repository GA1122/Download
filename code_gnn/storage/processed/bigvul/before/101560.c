void TestSwapCallback(void* user_data, int32_t result) {
  EXPECT(result == PP_OK);
  RenderInfo* info = static_cast<RenderInfo *>(user_data);
  glSetCurrentContextPPAPI(info->graphics3d_id);
  glViewport(0, 0, kWidth, kHeight);
  float blue = float(info->frame_counter) / float(info->frame_end);
  glClearColor(0.0f, 0.0f, blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  info->frame_counter += info->frame_increment;
  if (info->frame_counter < info->frame_end) {
    PP_CompletionCallback cc =
        PP_MakeCompletionCallback(TestSwapCallback, info);
    int32_t result = PPBGraphics3D()->SwapBuffers(info->graphics3d_id, cc);
    CHECK(PP_OK_COMPLETIONPENDING == result);
  } else {
    PPBCore()->ReleaseResource(info->graphics3d_id);
    delete info;
    TEST_PASSED;
  }
  glSetCurrentContextPPAPI(0);
}
