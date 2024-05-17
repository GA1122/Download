static const char *columnTypeImpl(
  NameContext *pNC, 
  Expr *pExpr,
#ifdef SQLITE_ENABLE_COLUMN_METADATA
  const char **pzOrigDb,
  const char **pzOrigTab,
  const char **pzOrigCol,
#endif
  u8 *pEstWidth
){
  char const *zType = 0;
  int j;
  u8 estWidth = 1;
#ifdef SQLITE_ENABLE_COLUMN_METADATA
  char const *zOrigDb = 0;
  char const *zOrigTab = 0;
  char const *zOrigCol = 0;
#endif

  assert( pExpr!=0 );
  assert( pNC->pSrcList!=0 );
  switch( pExpr->op ){
    case TK_AGG_COLUMN:
    case TK_COLUMN: {
       
      Table *pTab = 0;             
      Select *pS = 0;              
      int iCol = pExpr->iColumn;   
      testcase( pExpr->op==TK_AGG_COLUMN );
      testcase( pExpr->op==TK_COLUMN );
      while( pNC && !pTab ){
        SrcList *pTabList = pNC->pSrcList;
        for(j=0;j<pTabList->nSrc && pTabList->a[j].iCursor!=pExpr->iTable;j++);
        if( j<pTabList->nSrc ){
          pTab = pTabList->a[j].pTab;
          pS = pTabList->a[j].pSelect;
        }else{
          pNC = pNC->pNext;
        }
      }

      if( pTab==0 ){
         
        break;
      }

      assert( pTab && pExpr->pTab==pTab );
      if( pS ){
         
        if( iCol>=0 && ALWAYS(iCol<pS->pEList->nExpr) ){
           
          NameContext sNC;
          Expr *p = pS->pEList->a[iCol].pExpr;
          sNC.pSrcList = pS->pSrc;
          sNC.pNext = pNC;
          sNC.pParse = pNC->pParse;
          zType = columnType(&sNC, p,&zOrigDb,&zOrigTab,&zOrigCol, &estWidth); 
        }
      }else if( pTab->pSchema ){
         
        assert( !pS );
        if( iCol<0 ) iCol = pTab->iPKey;
        assert( iCol==-1 || (iCol>=0 && iCol<pTab->nCol) );
#ifdef SQLITE_ENABLE_COLUMN_METADATA
        if( iCol<0 ){
          zType = "INTEGER";
          zOrigCol = "rowid";
        }else{
          zOrigCol = pTab->aCol[iCol].zName;
          zType = sqlite3ColumnType(&pTab->aCol[iCol],0);
          estWidth = pTab->aCol[iCol].szEst;
        }
        zOrigTab = pTab->zName;
        if( pNC->pParse ){
          int iDb = sqlite3SchemaToIndex(pNC->pParse->db, pTab->pSchema);
          zOrigDb = pNC->pParse->db->aDb[iDb].zDbSName;
        }
#else
        if( iCol<0 ){
          zType = "INTEGER";
        }else{
          zType = sqlite3ColumnType(&pTab->aCol[iCol],0);
          estWidth = pTab->aCol[iCol].szEst;
        }
#endif
      }
      break;
    }
#ifndef SQLITE_OMIT_SUBQUERY
    case TK_SELECT: {
       
      NameContext sNC;
      Select *pS = pExpr->x.pSelect;
      Expr *p = pS->pEList->a[0].pExpr;
      assert( ExprHasProperty(pExpr, EP_xIsSelect) );
      sNC.pSrcList = pS->pSrc;
      sNC.pNext = pNC;
      sNC.pParse = pNC->pParse;
      zType = columnType(&sNC, p, &zOrigDb, &zOrigTab, &zOrigCol, &estWidth); 
      break;
    }
#endif
  }

#ifdef SQLITE_ENABLE_COLUMN_METADATA  
  if( pzOrigDb ){
    assert( pzOrigTab && pzOrigCol );
    *pzOrigDb = zOrigDb;
    *pzOrigTab = zOrigTab;
    *pzOrigCol = zOrigCol;
  }
#endif
  if( pEstWidth ) *pEstWidth = estWidth;
  return zType;
}
