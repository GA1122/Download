void HomePageUndoBubble::ShowBubble(Browser* browser,
                                    bool undo_value_is_ntp,
                                    const GURL& undo_url,
                                    views::View* anchor_view) {
  HideBubble();
  home_page_undo_bubble_ = new HomePageUndoBubble(browser,
                                                  undo_value_is_ntp,
                                                  undo_url,
                                                  anchor_view);
  views::BubbleDelegateView::CreateBubble(home_page_undo_bubble_);
  home_page_undo_bubble_->StartFade(true);
}
