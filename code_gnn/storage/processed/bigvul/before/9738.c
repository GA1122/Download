parse_register_dcl(
   struct translate_ctx *ctx,
   uint *file,
   struct parsed_dcl_bracket *brackets,
   int *num_brackets)
{
   const char *cur;

   *num_brackets = 0;

   if (!parse_register_file_bracket( ctx, file ))
      return FALSE;
   if (!parse_register_dcl_bracket( ctx, &brackets[0] ))
      return FALSE;

   *num_brackets = 1;

   cur = ctx->cur;
   eat_opt_white( &cur );

   if (cur[0] == '[') {
      bool is_in = *file == TGSI_FILE_INPUT;
      bool is_out = *file == TGSI_FILE_OUTPUT;

      ++cur;
      ctx->cur = cur;
      if (!parse_register_dcl_bracket( ctx, &brackets[1] ))
         return FALSE;
       

       
      if ((ctx->processor == TGSI_PROCESSOR_GEOMETRY && is_in) ||
          (ctx->processor == TGSI_PROCESSOR_TESS_EVAL && is_in) ||
          (ctx->processor == TGSI_PROCESSOR_TESS_CTRL && (is_in || is_out))) {
         brackets[0] = brackets[1];
         *num_brackets = 1;
      } else {
         *num_brackets = 2;
      }
   }

   return TRUE;
}
