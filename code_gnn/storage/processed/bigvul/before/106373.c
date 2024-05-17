LoginHtmlDialog::LoginHtmlDialog(Delegate* delegate,
                                 gfx::NativeWindow parent_window,
                                 const std::wstring& title,
                                 const GURL& url,
                                 Style style)
    : delegate_(delegate),
      parent_window_(parent_window),
      title_(title),
      url_(url),
      style_(style),
      bubble_frame_view_(NULL),
      is_open_(false) {
  gfx::Rect screen_bounds(chromeos::CalculateScreenBounds(gfx::Size()));
  width_ = static_cast<int>(kDefaultWidthRatio * screen_bounds.width());
  height_ = static_cast<int>(kDefaultHeightRatio * screen_bounds.height());
}
