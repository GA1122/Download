void RenderMessageFilter::OnPreCacheFontCharacters(const LOGFONT& font,
                                                   const string16& str) {
  HDC hdc = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
  HFONT font_handle = CreateFontIndirect(&font);
  DCHECK(NULL != font_handle);

  HGDIOBJ old_font = SelectObject(hdc, font_handle);
  DCHECK(NULL != old_font);

  ExtTextOut(hdc, 0, 0, ETO_GLYPH_INDEX, 0, str.c_str(), str.length(), NULL);

  SelectObject(hdc, old_font);
  DeleteObject(font_handle);

  HENHMETAFILE metafile = CloseEnhMetaFile(hdc);

  if (metafile) {
    DeleteEnhMetaFile(metafile);
  }
}
