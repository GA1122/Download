autofill::SaveCardBubbleView* BrowserView::ShowSaveCreditCardBubble(
    content::WebContents* web_contents,
    autofill::SaveCardBubbleController* controller,
    bool user_gesture) {
  LocationBarView* location_bar = GetLocationBarView();
  PageActionIconView* card_view = location_bar->save_credit_card_icon_view();

  views::View* anchor_view = location_bar;
  if (!ui::MaterialDesignController::IsSecondaryUiMaterial()) {
    if (card_view && card_view->visible())
      anchor_view = card_view;
    else
      anchor_view = toolbar_button_provider()->GetAppMenuButton();
  }

  autofill::SaveCardBubbleViews* bubble = new autofill::SaveCardBubbleViews(
      anchor_view, gfx::Point(), web_contents, controller);
  views::Widget* bubble_widget =
      views::BubbleDialogDelegateView::CreateBubble(bubble);

  if (card_view)
    card_view->OnBubbleWidgetCreated(bubble_widget);
  bubble->Show(user_gesture ? autofill::SaveCardBubbleViews::USER_GESTURE
                            : autofill::SaveCardBubbleViews::AUTOMATIC);
  return bubble;
}
