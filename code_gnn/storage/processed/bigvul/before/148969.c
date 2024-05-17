static VdbeCursor *allocateCursor(
  Vdbe *p,               
  int iCur,              
  int nField,            
  int iDb,               
  u8 eCurType            
){
   
  Mem *pMem = iCur>0 ? &p->aMem[p->nMem-iCur] : p->aMem;

  int nByte;
  VdbeCursor *pCx = 0;
  nByte = 
      ROUND8(sizeof(VdbeCursor)) + 2*sizeof(u32)*nField + 
      (eCurType==CURTYPE_BTREE?sqlite3BtreeCursorSize():0);

  assert( iCur>=0 && iCur<p->nCursor );
  if( p->apCsr[iCur] ){  
    sqlite3VdbeFreeCursor(p, p->apCsr[iCur]);
    p->apCsr[iCur] = 0;
  }
  if( SQLITE_OK==sqlite3VdbeMemClearAndResize(pMem, nByte) ){
    p->apCsr[iCur] = pCx = (VdbeCursor*)pMem->z;
    memset(pCx, 0, offsetof(VdbeCursor,pAltCursor));
    pCx->eCurType = eCurType;
    pCx->iDb = iDb;
    pCx->nField = nField;
    pCx->aOffset = &pCx->aType[nField];
    if( eCurType==CURTYPE_BTREE ){
      pCx->uc.pCursor = (BtCursor*)
          &pMem->z[ROUND8(sizeof(VdbeCursor))+2*sizeof(u32)*nField];
      sqlite3BtreeCursorZero(pCx->uc.pCursor);
    }
  }
  return pCx;
}
