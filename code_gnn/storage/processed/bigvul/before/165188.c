static std::unique_ptr<DragImage> DragImageForLink(const KURL& link_url,
                                                   const String& link_text,
                                                   float device_scale_factor) {
  FontDescription font_description;
  LayoutTheme::GetTheme().SystemFont(blink::CSSValueNone, font_description);
  return DragImage::Create(link_url, link_text, font_description,
                           device_scale_factor);
}
