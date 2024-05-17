void Browser::UpdateTargetURLHelper(TabContents* tab, int32 page_id,
                                    const GURL& url) {
  TabContentsWrapper* tcw = TabContentsWrapper::GetCurrentWrapperForContents(
      tab);
  if (!tcw || !tcw->prerender_tab_helper())
    return;
  tcw->prerender_tab_helper()->UpdateTargetURL(page_id, url);
}
