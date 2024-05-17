static int vrend_decode_set_polygon_stipple(struct vrend_decode_ctx *ctx, int length)
{
   struct pipe_poly_stipple ps;
   int i;

   if (length != VIRGL_POLYGON_STIPPLE_SIZE)
      return EINVAL;

   for (i = 0; i < 32; i++)
      ps.stipple[i] = get_buf_entry(ctx, VIRGL_POLYGON_STIPPLE_P0 + i);

   vrend_set_polygon_stipple(ctx->grctx, &ps);
   return 0;
}
