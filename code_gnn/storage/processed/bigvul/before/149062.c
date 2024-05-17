static void checkPage(PgHdr *pPg){
  Pager *pPager = pPg->pPager;
  assert( pPager->eState!=PAGER_ERROR );
  assert( (pPg->flags&PGHDR_DIRTY) || pPg->pageHash==pager_pagehash(pPg) );
}
