void Browser::BookmarkAllTabs() {
  BookmarkModel* model = profile()->GetBookmarkModel();
  DCHECK(model && model->IsLoaded());

  BookmarkEditor::EditDetails details;
  details.type = BookmarkEditor::EditDetails::NEW_FOLDER;
  bookmark_utils::GetURLsForOpenTabs(this, &(details.urls));
  DCHECK(!details.urls.empty());

  BookmarkEditor::Show(window()->GetNativeHandle(), profile_,
                       model->GetParentForNewNodes(),  details,
                       BookmarkEditor::SHOW_TREE);
}
