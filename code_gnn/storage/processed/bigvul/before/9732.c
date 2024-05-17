parse_opt_writemask(
   struct translate_ctx *ctx,
   uint *writemask )
{
   const char *cur;

   cur = ctx->cur;
   eat_opt_white( &cur );
   if (*cur == '.') {
      cur++;
      *writemask = TGSI_WRITEMASK_NONE;
      eat_opt_white( &cur );
      if (uprcase( *cur ) == 'X') {
         cur++;
         *writemask |= TGSI_WRITEMASK_X;
      }
      if (uprcase( *cur ) == 'Y') {
         cur++;
         *writemask |= TGSI_WRITEMASK_Y;
      }
      if (uprcase( *cur ) == 'Z') {
         cur++;
         *writemask |= TGSI_WRITEMASK_Z;
      }
      if (uprcase( *cur ) == 'W') {
         cur++;
         *writemask |= TGSI_WRITEMASK_W;
      }

      if (*writemask == TGSI_WRITEMASK_NONE) {
         report_error( ctx, "Writemask expected" );
         return FALSE;
      }

      ctx->cur = cur;
   }
   else {
      *writemask = TGSI_WRITEMASK_XYZW;
   }
   return TRUE;
}
