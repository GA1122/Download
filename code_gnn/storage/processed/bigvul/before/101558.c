 void TestResizeBuffersWithoutDepthBuffer() {
   int32_t attribs[] = {
      PP_GRAPHICS3DATTRIB_WIDTH, kWidth,
      PP_GRAPHICS3DATTRIB_HEIGHT, kHeight,
      PP_GRAPHICS3DATTRIB_NONE};
   TestResizeBuffers(attribs);
 }
