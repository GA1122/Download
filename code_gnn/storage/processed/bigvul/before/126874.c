void BrowserView::TabDeactivated(TabContents* contents) {
  if (!contents->in_destructor())
    contents->web_contents()->GetView()->StoreFocus();
}
