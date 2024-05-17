fix_glyph_scaling(ASS_Renderer *priv, GlyphInfo *glyph)
{
    double ft_size;
    if (priv->settings.hinting == ASS_HINTING_NONE) {
        ft_size = 256.0;
    } else {
        ft_size = glyph->scale_y * glyph->font_size;
    }
    glyph->scale_x = glyph->scale_x * glyph->font_size / ft_size;
    glyph->scale_y = glyph->scale_y * glyph->font_size / ft_size;
    glyph->font_size = ft_size;
}
