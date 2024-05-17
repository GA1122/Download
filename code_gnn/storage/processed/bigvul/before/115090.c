void BackgroundContentsService::LoadBackgroundContentsFromPrefs(
    Profile* profile) {
  if (!prefs_)
    return;
  const DictionaryValue* contents =
      prefs_->GetDictionary(prefs::kRegisteredBackgroundContents);
  if (!contents)
    return;
  ExtensionService* extensions_service = profile->GetExtensionService();
  DCHECK(extensions_service);
  for (DictionaryValue::key_iterator it = contents->begin_keys();
       it != contents->end_keys(); ++it) {
    const Extension* extension = extensions_service->GetExtensionById(
        *it, false);
    if (!extension) {
      NOTREACHED() << "No extension found for BackgroundContents - id = "
                   << *it;
      continue;
    }
    LoadBackgroundContentsFromDictionary(profile, *it, contents);
  }
}
