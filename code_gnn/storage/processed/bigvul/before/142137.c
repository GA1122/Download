  bool URLTFHasParent() {
    if (editor_->details_.type == BookmarkEditor::EditDetails::NEW_FOLDER)
      return false;
    return editor_->url_tf_->parent();
  }
