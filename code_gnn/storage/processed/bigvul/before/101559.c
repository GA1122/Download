void TestSwapBuffers() {
  int32_t attribs[] = {
      PP_GRAPHICS3DATTRIB_WIDTH, kWidth,
      PP_GRAPHICS3DATTRIB_HEIGHT, kHeight,
      PP_GRAPHICS3DATTRIB_NONE};
  PP_Resource graphics3d_id = PPBGraphics3D()->
      Create(pp_instance(), kInvalidResource, attribs);
  EXPECT(graphics3d_id != kInvalidResource);
  int32_t success = PPBInstance()->BindGraphics(pp_instance(), graphics3d_id);
  EXPECT(success == PP_TRUE);
  RenderInfo* render_info = new RenderInfo;
  render_info->graphics3d_id = graphics3d_id;
  render_info->frame_counter = 0;
  render_info->frame_end = 256;
  render_info->frame_increment = 2;
  PP_CompletionCallback cc = PP_MakeCompletionCallback(
      TestSwapCallback, render_info);
  PPBCore()->CallOnMainThread(0, cc, PP_OK);
 }
