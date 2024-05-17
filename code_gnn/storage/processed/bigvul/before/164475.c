  static void assertCellInfo(BtCursor *pCur){
    CellInfo info;
    memset(&info, 0, sizeof(info));
    btreeParseCell(pCur->pPage, pCur->ix, &info);
    assert( CORRUPT_DB || cellInfoEqual(&info, &pCur->info) );
  }
