void AppendToTabIdList(Browser* browser, ListValue* tab_ids) {
  DCHECK(browser);
  DCHECK(tab_ids);
  TabStripModel* tab_strip = browser->tabstrip_model();
  for (int i = 0; i < tab_strip->count(); ++i) {
    tab_ids->Append(Value::CreateIntegerValue(
        ExtensionTabUtil::GetTabId(tab_strip->GetTabContentsAt(i))));
  }
}
