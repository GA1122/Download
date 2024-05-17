  int GetID(TabContents* contents) {
    return *GetIDAccessor()->GetProperty(contents->property_bag());
  }
