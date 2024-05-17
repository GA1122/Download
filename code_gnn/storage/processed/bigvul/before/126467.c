void TabContentsContainerGtk::PackTab(TabContents* tab) {
  gfx::NativeView widget = tab->web_contents()->GetNativeView();
  if (widget) {
    if (gtk_widget_get_parent(widget) != expanded_)
      gtk_container_add(GTK_CONTAINER(expanded_), widget);
    gtk_widget_show(widget);
  }

  tab->web_contents()->WasShown();
  registrar_.Add(this, content::NOTIFICATION_WEB_CONTENTS_DESTROYED,
                 content::Source<WebContents>(tab->web_contents()));
}
