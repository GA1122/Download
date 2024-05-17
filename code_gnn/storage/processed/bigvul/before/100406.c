void RenderViewHostDelegateViewHelper::CreateNewWindow(
    int route_id,
    Profile* profile,
    SiteInstance* site,
    DOMUITypeID domui_type,
    TabContents* old_tab_contents) {
  TabContents* new_contents =
      new TabContents(profile,
                      site,
                      route_id,
                      old_tab_contents);
  new_contents->set_opener_dom_ui_type(domui_type);
  TabContentsView* new_view = new_contents->view();

  new_view->CreateViewForWidget(new_contents->render_view_host());

  pending_contents_[route_id] = new_contents;
}
