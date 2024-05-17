static void lite_font_init( wmfAPI* API, wmfAPI_Options* options)
{
  wmfFontData
    *font_data;

  (void) options;
  API->fonts = 0;

   
  API->font_data = wmf_malloc(API,sizeof(wmfFontData));
  if (ERR (API))
    return;

  font_data = (wmfFontData*)API->font_data;

   
  font_data->map = lite_font_map;

   
  font_data->stringwidth = lite_font_stringwidth;

   
  font_data->user_data = wmf_malloc(API,sizeof(wmf_magick_font_t));
  if (ERR(API))
    return;
  ((wmf_magick_font_t*)font_data->user_data)->ps_name = 0;
  ((wmf_magick_font_t*)font_data->user_data)->pointsize = 0;
}