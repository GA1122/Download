void Browser::TabDetachedAt(TabContentsWrapper* contents, int index) {
  TabDetachedAtImpl(contents, index, DETACH_TYPE_DETACH);
}
