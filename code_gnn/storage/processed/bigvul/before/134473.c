HomePageUndoBubble::HomePageUndoBubble(
    Browser* browser,
    bool undo_value_is_ntp,
    const GURL& undo_url,
    views::View* anchor_view)
    : BubbleDelegateView(anchor_view, views::BubbleBorder::TOP_LEFT),
      browser_(browser),
      undo_value_is_ntp_(undo_value_is_ntp),
      undo_url_(undo_url) {
}
