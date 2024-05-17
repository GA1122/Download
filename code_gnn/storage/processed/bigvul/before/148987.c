static void assertTruncateConstraintCb(PgHdr *pPg){
  assert( pPg->flags&PGHDR_DIRTY );
  assert( !subjRequiresPage(pPg) || pPg->pgno<=pPg->pPager->dbSize );
}
