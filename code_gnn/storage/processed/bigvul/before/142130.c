  void CreateEditor(Profile* profile,
                    const BookmarkNode* parent,
                    const BookmarkEditor::EditDetails& details,
                    BookmarkEditor::Configuration configuration) {
    editor_.reset(new BookmarkEditorView(profile, parent, details,
                                         configuration));
  }
