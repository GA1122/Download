static SQLITE_NOINLINE int accessPayloadChecked(
  BtCursor *pCur,
  u32 offset,
  u32 amt,
  void *pBuf
){
  int rc;
  if ( pCur->eState==CURSOR_INVALID ){
    return SQLITE_ABORT;
  }
  assert( cursorOwnsBtShared(pCur) );
  rc = btreeRestoreCursorPosition(pCur);
  return rc ? rc : accessPayload(pCur, offset, amt, pBuf, 0);
}
