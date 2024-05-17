static void codeAttach(
  Parse *pParse,        
  int type,             
  FuncDef const *pFunc, 
  Expr *pAuthArg,       
  Expr *pFilename,      
  Expr *pDbname,        
  Expr *pKey            
){
  int rc;
  NameContext sName;
  Vdbe *v;
  sqlite3* db = pParse->db;
  int regArgs;

  if( pParse->nErr ) goto attach_end;
  memset(&sName, 0, sizeof(NameContext));
  sName.pParse = pParse;

  if(
      SQLITE_OK!=(rc = resolveAttachExpr(&sName, pFilename)) ||
      SQLITE_OK!=(rc = resolveAttachExpr(&sName, pDbname)) ||
      SQLITE_OK!=(rc = resolveAttachExpr(&sName, pKey))
  ){
    goto attach_end;
  }

#ifndef SQLITE_OMIT_AUTHORIZATION
  if( pAuthArg ){
    char *zAuthArg;
    if( pAuthArg->op==TK_STRING ){
      zAuthArg = pAuthArg->u.zToken;
    }else{
      zAuthArg = 0;
    }
    rc = sqlite3AuthCheck(pParse, type, zAuthArg, 0, 0);
    if(rc!=SQLITE_OK ){
      goto attach_end;
    }
  }
#endif  


  v = sqlite3GetVdbe(pParse);
  regArgs = sqlite3GetTempRange(pParse, 4);
  sqlite3ExprCode(pParse, pFilename, regArgs);
  sqlite3ExprCode(pParse, pDbname, regArgs+1);
  sqlite3ExprCode(pParse, pKey, regArgs+2);

  assert( v || db->mallocFailed );
  if( v ){
    sqlite3VdbeAddOp4(v, OP_Function0, 0, regArgs+3-pFunc->nArg, regArgs+3,
                      (char *)pFunc, P4_FUNCDEF);
    assert( pFunc->nArg==-1 || (pFunc->nArg&0xff)==pFunc->nArg );
    sqlite3VdbeChangeP5(v, (u8)(pFunc->nArg));

     
    sqlite3VdbeAddOp1(v, OP_Expire, (type==SQLITE_ATTACH));
  }

attach_end:
  sqlite3ExprDelete(db, pFilename);
  sqlite3ExprDelete(db, pDbname);
  sqlite3ExprDelete(db, pKey);
}
