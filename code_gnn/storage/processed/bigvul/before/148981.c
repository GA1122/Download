static void applyAffinity(
  Mem *pRec,           
  char affinity,       
  u8 enc               
){
  if( affinity>=SQLITE_AFF_NUMERIC ){
    assert( affinity==SQLITE_AFF_INTEGER || affinity==SQLITE_AFF_REAL
             || affinity==SQLITE_AFF_NUMERIC );
    if( (pRec->flags & MEM_Int)==0 ){  
      if( (pRec->flags & MEM_Real)==0 ){
        if( pRec->flags & MEM_Str ) applyNumericAffinity(pRec,1);
      }else{
        sqlite3VdbeIntegerAffinity(pRec);
      }
    }
  }else if( affinity==SQLITE_AFF_TEXT ){
     
    if( 0==(pRec->flags&MEM_Str) ){  
      if( (pRec->flags&(MEM_Real|MEM_Int)) ){
        sqlite3VdbeMemStringify(pRec, enc, 1);
      }
    }
    pRec->flags &= ~(MEM_Real|MEM_Int);
  }
}
