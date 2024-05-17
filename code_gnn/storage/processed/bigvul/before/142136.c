  void SetURLText(const base::string16& text) {
    if (editor_->details_.type != BookmarkEditor::EditDetails::NEW_FOLDER)
       editor_->url_tf_->SetText(text);
   }
