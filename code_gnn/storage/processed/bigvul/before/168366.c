autofill::LocalCardMigrationBubble* BrowserView::ShowLocalCardMigrationBubble(
    content::WebContents* web_contents,
    autofill::LocalCardMigrationBubbleController* controller,
    bool user_gesture) {
  LocationBarView* location_bar = GetLocationBarView();
  PageActionIconView* card_view =
      location_bar->local_card_migration_icon_view();

  views::View* anchor_view = location_bar;
  if (!ui::MaterialDesignController::IsSecondaryUiMaterial()) {
    if (card_view && card_view->visible())
      anchor_view = card_view;
    else
      anchor_view = toolbar_button_provider()->GetAppMenuButton();
  }

  autofill::LocalCardMigrationBubbleViews* bubble =
      new autofill::LocalCardMigrationBubbleViews(anchor_view, gfx::Point(),
                                                  web_contents, controller);
  views::Widget* bubble_widget =
      views::BubbleDialogDelegateView::CreateBubble(bubble);
  if (card_view)
    card_view->OnBubbleWidgetCreated(bubble_widget);
  bubble->Show(user_gesture
                   ? autofill::LocalCardMigrationBubbleViews::USER_GESTURE
                   : autofill::LocalCardMigrationBubbleViews::AUTOMATIC);
  return bubble;
}
