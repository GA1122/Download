void GCInfoTable::init() {
  RELEASE_ASSERT(!s_gcInfoTable);
  resize();
}
