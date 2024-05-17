void BrowserEventRouter::TabReplacedAt(TabStripModel* tab_strip_model,
                                       TabContents* old_contents,
                                       TabContents* new_contents,
                                       int index) {
  TabClosingAt(tab_strip_model, old_contents->web_contents(), index);
  TabInsertedAt(new_contents->web_contents(),
                index,
                tab_strip_model->active_index() == index);
}
