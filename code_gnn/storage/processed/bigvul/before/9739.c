parse_register_dcl_bracket(
   struct translate_ctx *ctx,
   struct parsed_dcl_bracket *bracket)
{
   uint uindex;
   memset(bracket, 0, sizeof(struct parsed_dcl_bracket));

   eat_opt_white( &ctx->cur );

   if (!parse_uint( &ctx->cur, &uindex )) {
       
      if (ctx->cur[0] == ']' && ctx->implied_array_size != 0) {
         bracket->first = 0;
         bracket->last = ctx->implied_array_size - 1;
         goto cleanup;
      }
      report_error( ctx, "Expected literal unsigned integer" );
      return FALSE;
   }
   bracket->first = uindex;

   eat_opt_white( &ctx->cur );

   if (ctx->cur[0] == '.' && ctx->cur[1] == '.') {
      uint uindex;

      ctx->cur += 2;
      eat_opt_white( &ctx->cur );
      if (!parse_uint( &ctx->cur, &uindex )) {
         report_error( ctx, "Expected literal integer" );
         return FALSE;
      }
      bracket->last = (int) uindex;
      eat_opt_white( &ctx->cur );
   }
   else {
      bracket->last = bracket->first;
   }

cleanup:
   if (*ctx->cur != ']') {
      report_error( ctx, "Expected `]' or `..'" );
      return FALSE;
   }
   ctx->cur++;
   return TRUE;
}
