 jbig2_sd_cat(Jbig2Ctx *ctx, int n_dicts, Jbig2SymbolDict **dicts)
// jbig2_sd_cat(Jbig2Ctx *ctx, uint32_t n_dicts, Jbig2SymbolDict **dicts)
  {
    int i, j, k, symbols;
    Jbig2SymbolDict *new = NULL;
//     uint32_t i, j, k, symbols;
//     Jbig2SymbolDict *new_dict = NULL;
  
       
      symbols = 0;
     for (i = 0; i < n_dicts; i++)
          symbols += dicts[i]->n_symbols;
  
       
    new = jbig2_sd_new(ctx, symbols);
    if (new != NULL) {
//     new_dict = jbig2_sd_new(ctx, symbols);
//     if (new_dict != NULL) {
          k = 0;
          for (i = 0; i < n_dicts; i++)
              for (j = 0; j < dicts[i]->n_symbols; j++)
                new->glyphs[k++] = jbig2_image_clone(ctx, dicts[i]->glyphs[j]);
//                 new_dict->glyphs[k++] = jbig2_image_clone(ctx, dicts[i]->glyphs[j]);
      } else {
          jbig2_error(ctx, JBIG2_SEVERITY_WARNING, -1, "failed to allocate new symbol dictionary");
      }
  
    return new;
//     return new_dict;
  }