void AeroPeekManager::ActivateTab(int tab_id) {
  TabContents* contents = GetTabContents(tab_id);
  if (contents && contents->delegate())
    contents->delegate()->ActivateContents(contents);
}
