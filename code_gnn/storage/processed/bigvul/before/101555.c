void TestIsGraphics3D() {
  int32_t attribs[] = {
      PP_GRAPHICS3DATTRIB_WIDTH, kWidth,
      PP_GRAPHICS3DATTRIB_HEIGHT, kHeight,
      PP_GRAPHICS3DATTRIB_NONE};
  PP_Resource graphics3d_id = PPBGraphics3D()->
      Create(pp_instance(), kInvalidResource, attribs);
  EXPECT(graphics3d_id != kInvalidResource);
  EXPECT(PPBGraphics3D()->IsGraphics3D(graphics3d_id) == PP_TRUE);
  PPBCore()->ReleaseResource(graphics3d_id);
   TEST_PASSED;
 }
