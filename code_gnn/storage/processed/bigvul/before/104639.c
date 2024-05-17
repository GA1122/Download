Extension::FileBrowserHandlerList* Extension::LoadFileBrowserHandlers(
    const ListValue* extension_actions, std::string* error) {
  scoped_ptr<FileBrowserHandlerList> result(
      new FileBrowserHandlerList());
  for (ListValue::const_iterator iter = extension_actions->begin();
       iter != extension_actions->end();
       ++iter) {
    if (!(*iter)->IsType(Value::TYPE_DICTIONARY)) {
      *error = errors::kInvalidFileBrowserHandler;
      return NULL;
    }
    scoped_ptr<FileBrowserHandler> action(
        LoadFileBrowserHandler(
            reinterpret_cast<DictionaryValue*>(*iter), error));
    if (!action.get())
      return NULL;   
    result->push_back(linked_ptr<FileBrowserHandler>(action.release()));
  }
  return result.release();
}
