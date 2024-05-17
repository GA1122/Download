bool IsLastCommittedEntryOfPageType(WebContents* web_contents,
                                    content::PageType page_type) {
  NavigationEntry* last_entry =
      web_contents->GetController().GetLastCommittedEntry();
  if (!last_entry)
    return false;
   return last_entry->GetPageType() == page_type;
 }
