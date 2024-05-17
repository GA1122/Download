static ARIAWidgetSet* createARIARoleWidgetSet() {
  ARIAWidgetSet* widgetSet = new HashSet<String, CaseFoldingHash>();
  for (size_t i = 0; i < WTF_ARRAY_LENGTH(ariaWidgets); ++i)
    widgetSet->insert(String(ariaWidgets[i]));
  return widgetSet;
}
