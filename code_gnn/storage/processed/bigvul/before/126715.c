void BrowserView::ActiveTabChanged(TabContents* old_contents,
                                   TabContents* new_contents,
                                   int index,
                                   bool user_gesture) {
  DCHECK(new_contents);

  if (contents_->preview_web_contents() == new_contents->web_contents()) {
    contents_->MakePreviewContentsActiveContents();
    views::WebView* old_container = contents_container_;
    contents_container_ = preview_controller_->release_preview_container();
    old_container->SetWebContents(NULL);
    delete old_container;
  }

  bool change_tab_contents =
      contents_container_->web_contents() != new_contents->web_contents();


  if (change_tab_contents)
    contents_container_->SetWebContents(NULL);
  InfoBarTabHelper* new_infobar_tab_helper =
      InfoBarTabHelper::FromWebContents(new_contents->web_contents());
  infobar_container_->ChangeTabContents(new_infobar_tab_helper);
  if (bookmark_bar_view_.get()) {
    bookmark_bar_view_->SetBookmarkBarState(
        browser_->bookmark_bar_state(),
        BookmarkBar::DONT_ANIMATE_STATE_CHANGE,
        browser_->search_model()->mode());
  }
  UpdateUIForContents(new_contents);

  UpdateDevToolsForContents(new_contents);

  if (change_tab_contents) {
    contents_container_->SetWebContents(new_contents->web_contents());
#if defined(USE_AURA)
    if (contents_->preview_web_contents()) {
      ui::Layer* preview_layer =
          contents_->preview_web_contents()->GetNativeView()->layer();
      preview_layer->parent()->StackAtTop(preview_layer);
    }
#endif
  }

  if (!browser_->tab_strip_model()->closing_all() && GetWidget()->IsActive() &&
      GetWidget()->IsVisible()) {
    new_contents->web_contents()->GetView()->RestoreFocus();
  }

  UpdateTitleBar();

  MaybeStackBookmarkBarAtTop();

}
