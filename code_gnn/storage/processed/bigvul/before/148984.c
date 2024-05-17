  static void assertCellInfo(BtCursor *pCur){
    CellInfo info;
    int iPage = pCur->iPage;
    memset(&info, 0, sizeof(info));
    btreeParseCell(pCur->apPage[iPage], pCur->aiIdx[iPage], &info);
    assert( CORRUPT_DB || memcmp(&info, &pCur->info, sizeof(info))==0 );
  }
