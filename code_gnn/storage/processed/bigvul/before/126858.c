void BrowserView::ShowAvatarBubble(WebContents* web_contents,
                                   const gfx::Rect& rect) {
  gfx::Point origin(rect.origin());
  views::View::ConvertPointToScreen(GetTabContentsContainerView(), &origin);
  gfx::Rect bounds(origin, rect.size());

  AvatarMenuBubbleView* bubble = new AvatarMenuBubbleView(this,
      views::BubbleBorder::TOP_RIGHT, bounds, browser_.get());
  views::BubbleDelegateView::CreateBubble(bubble);
  bubble->SetAlignment(views::BubbleBorder::ALIGN_EDGE_TO_ANCHOR_EDGE);
  bubble->Show();
}
