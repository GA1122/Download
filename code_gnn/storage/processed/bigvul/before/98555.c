DictionaryValue* ExtensionTabUtil::CreateWindowValue(const Browser* browser,
                                                     bool populate_tabs) {
  DictionaryValue* result = new DictionaryValue();
  result->SetInteger(keys::kIdKey, ExtensionTabUtil::GetWindowId(browser));
  bool focused = false;
  if (browser->window())
    focused = browser->window()->IsActive();

  result->SetBoolean(keys::kIncognitoKey,
                     browser->profile()->IsOffTheRecord());
  result->SetBoolean(keys::kFocusedKey, focused);
  gfx::Rect bounds = browser->window()->GetRestoredBounds();

  result->SetInteger(keys::kLeftKey, bounds.x());
  result->SetInteger(keys::kTopKey, bounds.y());
  result->SetInteger(keys::kWidthKey, bounds.width());
  result->SetInteger(keys::kHeightKey, bounds.height());
  result->SetString(keys::kWindowTypeKey, GetWindowTypeText(browser->type()));

  if (populate_tabs) {
    result->Set(keys::kTabsKey, ExtensionTabUtil::CreateTabList(browser));
  }

  return result;
}
