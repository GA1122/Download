void SetPageSizeAndContentRect(bool rotated,
                               bool is_src_page_landscape,
                               pp::Size* page_size,
                               pp::Rect* content_rect) {
  bool is_dst_page_landscape = page_size->width() > page_size->height();
  bool page_orientation_mismatched = is_src_page_landscape !=
                                     is_dst_page_landscape;
  bool rotate_dst_page = rotated ^ page_orientation_mismatched;
  if (rotate_dst_page) {
    page_size->SetSize(page_size->height(), page_size->width());
    content_rect->SetRect(content_rect->y(), content_rect->x(),
                          content_rect->height(), content_rect->width());
  }
}
