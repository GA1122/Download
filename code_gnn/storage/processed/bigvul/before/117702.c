  virtual void StartDragging(const WebDropData& drop_data,
                             WebKit::WebDragOperationsMask allowed_ops,
                             const SkBitmap& image,
                             const gfx::Point& image_offset) {
    drag_url_ = drop_data.url;
    html_base_url_ = drop_data.html_base_url;
  }
