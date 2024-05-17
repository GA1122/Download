static u16 cellSizePtrNoPayload(MemPage *pPage, u8 *pCell){
  u8 *pIter = pCell + 4;  
  u8 *pEnd;               

#ifdef SQLITE_DEBUG
   
  CellInfo debuginfo;
  pPage->xParseCell(pPage, pCell, &debuginfo);
#else
  UNUSED_PARAMETER(pPage);
#endif

  assert( pPage->childPtrSize==4 );
  pEnd = pIter + 9;
  while( (*pIter++)&0x80 && pIter<pEnd );
  assert( debuginfo.nSize==(u16)(pIter - pCell) || CORRUPT_DB );
  return (u16)(pIter - pCell);
}
