void RenderThreadImpl::OnSystemColorsChanged(
    int32_t aqua_color_variant,
    const std::string& highlight_text_color,
    const std::string& highlight_color) {
#if defined(OS_MACOSX)
  SystemColorsDidChange(aqua_color_variant, highlight_text_color,
                        highlight_color);
#else
  NOTREACHED();
#endif
}
