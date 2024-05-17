void TabStripGtk::ActiveTabChanged(TabContents* old_contents,
                                   TabContents* new_contents,
                                   int index,
                                   bool user_gesture) {
  TRACE_EVENT0("ui::gtk", "TabStripGtk::ActiveTabChanged");
  ReStack();
}
