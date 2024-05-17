void TabContentsContainerGtk::HideTab(TabContents* tab) {
  gfx::NativeView widget = tab->web_contents()->GetNativeView();
  if (widget)
    gtk_widget_hide(widget);

  tab->web_contents()->WasHidden();
  registrar_.Remove(this, content::NOTIFICATION_WEB_CONTENTS_DESTROYED,
                     content::Source<WebContents>(tab->web_contents()));
 }
