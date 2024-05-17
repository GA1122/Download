void Browser::ViewSourceForFrame(TabContents* source,
                                 const GURL& frame_url,
                                 const std::string& frame_content_state) {
  DCHECK(source);
  int index = tabstrip_model()->GetWrapperIndex(source);
  TabContentsWrapper* wrapper = tabstrip_model()->GetTabContentsAt(index);
  ViewSource(wrapper, frame_url, frame_content_state);
}
