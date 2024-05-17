void PageSnapshotTaker::Start() {
  AutomationTabHelper* automation_tab_helper =
      AutomationTabHelper::FromWebContents(tab_contents_->web_contents());
  StartObserving(automation_tab_helper);
  automation_tab_helper->SnapshotEntirePage();
}
