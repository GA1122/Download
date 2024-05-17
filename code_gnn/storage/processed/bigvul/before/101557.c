 void TestResizeBuffers(int32_t* attribs) {
   PP_Resource graphics3d_id = PPBGraphics3D()->
       Create(pp_instance(), kInvalidResource, attribs);
  EXPECT(graphics3d_id != kInvalidResource);
  int32_t success = PPBInstance()->BindGraphics(pp_instance(), graphics3d_id);
  EXPECT(success == PP_TRUE);
  int32_t result;
  result = PPBGraphics3D()->ResizeBuffers(graphics3d_id, -1000, 0);
  EXPECT(result == PP_ERROR_BADARGUMENT);
  result = PPBGraphics3D()->ResizeBuffers(graphics3d_id, 100, -1);
  EXPECT(result == PP_ERROR_BADARGUMENT);
  result = PPBGraphics3D()->ResizeBuffers(graphics3d_id, -1, -1);
  EXPECT(result == PP_ERROR_BADARGUMENT);
  RenderInfo* render_info = new RenderInfo;
  render_info->graphics3d_id = graphics3d_id;
  render_info->frame_counter = 0;
  render_info->frame_end = 256;
  render_info->frame_increment = 8;
  PP_CompletionCallback cc = PP_MakeCompletionCallback(
      TestResizeAndSwapCallback, render_info);
   PPBCore()->CallOnMainThread(0, cc, PP_OK);
 }
