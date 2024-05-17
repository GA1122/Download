void BrowserWindowGtk::ActiveTabChanged(TabContents* old_contents,
                                        TabContents* new_contents,
                                        int index,
                                        bool user_gesture) {
  TRACE_EVENT0("ui::gtk", "BrowserWindowGtk::ActiveTabChanged");
  if (old_contents && !old_contents->in_destructor())
    old_contents->web_contents()->GetView()->StoreFocus();

  UpdateDevToolsForContents(new_contents->web_contents());
  InfoBarTabHelper* new_infobar_tab_helper =
      InfoBarTabHelper::FromWebContents(new_contents->web_contents());
  infobar_container_->ChangeTabContents(new_infobar_tab_helper);
  contents_container_->SetTab(new_contents);

  if (!browser_->tab_strip_model()->closing_all()) {
    new_contents->web_contents()->GetView()->RestoreFocus();
    FindTabHelper* find_tab_helper =
        FindTabHelper::FromWebContents(new_contents->web_contents());
    if (find_tab_helper->find_ui_active())
      browser_->GetFindBarController()->find_bar()->SetFocusAndSelection();
  }

  UpdateTitleBar();
  MaybeShowBookmarkBar(false);
}
