static void blitter_set_rectangle(struct vrend_blitter_ctx *blit_ctx,
                                  int x1, int y1, int x2, int y2,
                                  float depth)
{
   int i;

    
   blit_ctx->vertices[0][0][0] = (float)x1 / blit_ctx->dst_width * 2.0f - 1.0f;  
   blit_ctx->vertices[0][0][1] = (float)y1 / blit_ctx->dst_height * 2.0f - 1.0f;  

   blit_ctx->vertices[1][0][0] = (float)x2 / blit_ctx->dst_width * 2.0f - 1.0f;  
   blit_ctx->vertices[1][0][1] = (float)y1 / blit_ctx->dst_height * 2.0f - 1.0f;  

   blit_ctx->vertices[2][0][0] = (float)x2 / blit_ctx->dst_width * 2.0f - 1.0f;  
   blit_ctx->vertices[2][0][1] = (float)y2 / blit_ctx->dst_height * 2.0f - 1.0f;  

   blit_ctx->vertices[3][0][0] = (float)x1 / blit_ctx->dst_width * 2.0f - 1.0f;  
   blit_ctx->vertices[3][0][1] = (float)y2 / blit_ctx->dst_height * 2.0f - 1.0f;  

   for (i = 0; i < 4; i++)
      blit_ctx->vertices[i][0][2] = depth;  

   glViewport(0, 0, blit_ctx->dst_width, blit_ctx->dst_height);
}