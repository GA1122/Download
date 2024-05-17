 ListValue* ExtensionTabUtil::CreateTabList(
    const Browser* browser,
    const Extension* extension) {
  ListValue* tab_list = new ListValue();
  TabStripModel* tab_strip = browser->tab_strip_model();
  for (int i = 0; i < tab_strip->count(); ++i) {
    tab_list->Append(CreateTabValue(tab_strip->GetWebContentsAt(i),
                                    tab_strip,
                                    i,
                                    extension));
  }

  return tab_list;
}
