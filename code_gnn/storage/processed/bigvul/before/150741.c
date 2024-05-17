  void SetPageInfoBubbleIdentityInfo(
      const PageInfoUI::IdentityInfo& identity_info) {
    static_cast<PageInfoBubbleView*>(PageInfoBubbleView::GetPageInfoBubble())
         ->SetIdentityInfo(identity_info);
   }
