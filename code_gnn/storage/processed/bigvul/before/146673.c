 void InternalPageInfoBubbleView::OnWidgetDestroying(views::Widget* widget) {
   g_shown_bubble_type = PageInfoBubbleView::BUBBLE_NONE;
   g_page_info_bubble = nullptr;
 }
