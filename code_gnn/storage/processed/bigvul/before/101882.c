void Browser::ViewSourceForTab(TabContents* source, const GURL& page_url) {
  DCHECK(source);
  int index = tabstrip_model()->GetWrapperIndex(source);
  TabContentsWrapper* wrapper = tabstrip_model()->GetTabContentsAt(index);
  ViewSource(wrapper);
}
