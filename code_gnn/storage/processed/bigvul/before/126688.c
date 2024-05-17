  void SetID(WebContents* contents, int id) {
    contents->SetUserData(&kTabStripModelTestIDUserDataKey,
                          new TabStripModelTestIDUserData(id));
  }
