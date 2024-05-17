static void btreeParseCell(
  MemPage *pPage,          
  int iCell,               
  CellInfo *pInfo          
){
  pPage->xParseCell(pPage, findCell(pPage, iCell), pInfo);
}
