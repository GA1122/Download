void AeroPeekManager::CloseTab(int tab_id) {
  TabContents* contents = GetTabContents(tab_id);
  if (contents && contents->delegate())
    contents->delegate()->CloseContents(contents);
}
