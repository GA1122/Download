parse_register_file_bracket_index(
   struct translate_ctx *ctx,
   uint *file,
   int *index )
{
   uint uindex;

   if (!parse_register_file_bracket( ctx, file ))
      return FALSE;
   eat_opt_white( &ctx->cur );
   if (!parse_uint( &ctx->cur, &uindex )) {
      report_error( ctx, "Expected literal unsigned integer" );
      return FALSE;
   }
   *index = (int) uindex;
   return TRUE;
}
