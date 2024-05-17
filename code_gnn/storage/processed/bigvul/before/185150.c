  void BrowserEventRouter::TabUpdated(WebContents* contents, bool did_navigate) {
    TabEntry* entry = GetTabEntry(contents);
  DictionaryValue* changed_properties = NULL;
//   scoped_ptr<DictionaryValue> changed_properties;
  
    DCHECK(entry);
  
    if (did_navigate)
    changed_properties = entry->DidNavigate(contents);
//     changed_properties.reset(entry->DidNavigate(contents));
    else
    changed_properties = entry->UpdateLoadState(contents);
//     changed_properties.reset(entry->UpdateLoadState(contents));
  
    if (changed_properties)
    DispatchTabUpdatedEvent(contents, changed_properties);
//     DispatchTabUpdatedEvent(contents, changed_properties.Pass());
  }