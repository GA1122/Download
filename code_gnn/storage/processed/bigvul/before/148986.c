static void assertTruncateConstraint(Pager *pPager){
  sqlite3PcacheIterateDirty(pPager->pPCache, assertTruncateConstraintCb);
}
