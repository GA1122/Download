  void SetID(TabContents* contents, int id) {
    GetIDAccessor()->SetProperty(contents->property_bag(), id);
  }
