bool TabStripModel::InternalCloseTabs(const std::vector<int>& indices,
                                      uint32 close_types) {
  if (indices.empty())
    return true;

  std::vector<WebContents*> closing_contentses;
  for (size_t i = 0; i < indices.size(); ++i)
    closing_contentses.push_back(GetWebContentsAtImpl(indices[i]));

  if (browser_shutdown::GetShutdownType() == browser_shutdown::NOT_VALID) {
    std::map<content::RenderProcessHost*, size_t> processes;
    for (size_t i = 0; i < indices.size(); ++i) {
      WebContents* closing_contents = GetWebContentsAtImpl(indices[i]);
      content::RenderProcessHost* process =
          closing_contents->GetRenderProcessHost();
      ++processes[process];
    }

    for (std::map<content::RenderProcessHost*, size_t>::iterator iter =
         processes.begin(); iter != processes.end(); ++iter) {
      iter->first->FastShutdownForPageCount(iter->second);
    }
  }

  bool retval = true;
  for (size_t i = 0; i < closing_contentses.size(); ++i) {
    WebContents* closing_contents = closing_contentses[i];
    int index = GetIndexOfWebContents(closing_contents);
    if (index == kNoTab)
      continue;

    closing_contents->OnCloseStarted();

    if (!closing_contents->GetClosedByUserGesture()) {
      closing_contents->SetClosedByUserGesture(
          close_types & CLOSE_USER_GESTURE);
    }

    if (delegate_->RunUnloadListenerBeforeClosing(closing_contents)) {
      retval = false;
      continue;
    }

    InternalCloseTab(closing_contents, index,
                     (close_types & CLOSE_CREATE_HISTORICAL_TAB) != 0);
  }

  return retval;
}
