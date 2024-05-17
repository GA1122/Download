static u16 SQLITE_NOINLINE computeNumericType(Mem *pMem){
  assert( (pMem->flags & (MEM_Int|MEM_Real))==0 );
  assert( (pMem->flags & (MEM_Str|MEM_Blob))!=0 );
  ExpandBlob(pMem);
  if( sqlite3AtoF(pMem->z, &pMem->u.r, pMem->n, pMem->enc)==0 ){
    return 0;
  }
  if( sqlite3Atoi64(pMem->z, &pMem->u.i, pMem->n, pMem->enc)==0 ){
    return MEM_Int;
  }
  return MEM_Real;
}
