TabContents* TabStripModel::DiscardTabContentsAt(int index) {
  DCHECK(ContainsIndex(index));
  if (active_index() == index)
    return NULL;

  TabContents* null_contents = TabContents::Factory::CreateTabContents(
      WebContents::Create(profile(),
                          NULL  ,
                          MSG_ROUTING_NONE,
                          NULL  ));
  TabContents* old_contents = GetTabContentsAtImpl(index);
  null_contents->web_contents()->GetController().CopyStateFrom(
      old_contents->web_contents()->GetController());
  ReplaceTabContentsAt(index, null_contents);
  contents_data_[index]->discarded = true;
  delete old_contents;
  return null_contents;
}
