parse_register_bracket(
   struct translate_ctx *ctx,
   struct parsed_bracket *brackets)
{
   const char *cur;
   uint uindex;

   memset(brackets, 0, sizeof(struct parsed_bracket));

   eat_opt_white( &ctx->cur );

   cur = ctx->cur;
   if (parse_file( &cur, &brackets->ind_file )) {
      if (!parse_register_1d( ctx, &brackets->ind_file,
                              &brackets->ind_index ))
         return FALSE;
      eat_opt_white( &ctx->cur );

      if (*ctx->cur == '.') {
         ctx->cur++;
         eat_opt_white(&ctx->cur);

         switch (uprcase(*ctx->cur)) {
         case 'X':
            brackets->ind_comp = TGSI_SWIZZLE_X;
            break;
         case 'Y':
            brackets->ind_comp = TGSI_SWIZZLE_Y;
            break;
         case 'Z':
            brackets->ind_comp = TGSI_SWIZZLE_Z;
            break;
         case 'W':
            brackets->ind_comp = TGSI_SWIZZLE_W;
            break;
         default:
            report_error(ctx, "Expected indirect register swizzle component `x', `y', `z' or `w'");
            return FALSE;
         }
         ctx->cur++;
         eat_opt_white(&ctx->cur);
      }

      if (*ctx->cur == '+' || *ctx->cur == '-')
         parse_int( &ctx->cur, &brackets->index );
      else
         brackets->index = 0;
   }
   else {
      if (!parse_uint( &ctx->cur, &uindex )) {
         report_error( ctx, "Expected literal unsigned integer" );
         return FALSE;
      }
      brackets->index = (int) uindex;
      brackets->ind_file = TGSI_FILE_NULL;
      brackets->ind_index = 0;
   }
   eat_opt_white( &ctx->cur );
   if (*ctx->cur != ']') {
      report_error( ctx, "Expected `]'" );
      return FALSE;
   }
   ctx->cur++;
   if (*ctx->cur == '(') {
      ctx->cur++;
      eat_opt_white( &ctx->cur );
      if (!parse_uint( &ctx->cur, &brackets->ind_array )) {
         report_error( ctx, "Expected literal unsigned integer" );
         return FALSE;
      }
      eat_opt_white( &ctx->cur );
      if (*ctx->cur != ')') {
         report_error( ctx, "Expected `)'" );
         return FALSE;
      }
      ctx->cur++;
   }
   return TRUE;
}
