static void attachFunc(
  sqlite3_context *context,
  int NotUsed,
  sqlite3_value **argv
){
  int i;
  int rc = 0;
  sqlite3 *db = sqlite3_context_db_handle(context);
  const char *zName;
  const char *zFile;
  char *zPath = 0;
  char *zErr = 0;
  unsigned int flags;
  Db *aNew;                  
  Db *pNew;                  
  char *zErrDyn = 0;
  sqlite3_vfs *pVfs;

  UNUSED_PARAMETER(NotUsed);
  zFile = (const char *)sqlite3_value_text(argv[0]);
  zName = (const char *)sqlite3_value_text(argv[1]);
  if( zFile==0 ) zFile = "";
  if( zName==0 ) zName = "";

#ifdef SQLITE_ENABLE_DESERIALIZE
# define REOPEN_AS_MEMDB(db)  (db->init.reopenMemdb)
#else
# define REOPEN_AS_MEMDB(db)  (0)
#endif

  if( REOPEN_AS_MEMDB(db) ){
     
    pVfs = sqlite3_vfs_find("memdb");
    if( pVfs==0 ) return;
    pNew = &db->aDb[db->init.iDb];
    if( pNew->pBt ) sqlite3BtreeClose(pNew->pBt);
    pNew->pBt = 0;
    pNew->pSchema = 0;
    rc = sqlite3BtreeOpen(pVfs, "x\0", db, &pNew->pBt, 0, SQLITE_OPEN_MAIN_DB);
  }else{
     
    if( db->nDb>=db->aLimit[SQLITE_LIMIT_ATTACHED]+2 ){
      zErrDyn = sqlite3MPrintf(db, "too many attached databases - max %d",
        db->aLimit[SQLITE_LIMIT_ATTACHED]
      );
      goto attach_error;
    }
    for(i=0; i<db->nDb; i++){
      char *z = db->aDb[i].zDbSName;
      assert( z && zName );
      if( sqlite3StrICmp(z, zName)==0 ){
        zErrDyn = sqlite3MPrintf(db, "database %s is already in use", zName);
        goto attach_error;
      }
    }

     
    if( db->aDb==db->aDbStatic ){
      aNew = sqlite3DbMallocRawNN(db, sizeof(db->aDb[0])*3 );
      if( aNew==0 ) return;
      memcpy(aNew, db->aDb, sizeof(db->aDb[0])*2);
    }else{
      aNew = sqlite3DbRealloc(db, db->aDb, sizeof(db->aDb[0])*(db->nDb+1) );
      if( aNew==0 ) return;
    }
    db->aDb = aNew;
    pNew = &db->aDb[db->nDb];
    memset(pNew, 0, sizeof(*pNew));

     
    flags = db->openFlags;
    rc = sqlite3ParseUri(db->pVfs->zName, zFile, &flags, &pVfs, &zPath, &zErr);
    if( rc!=SQLITE_OK ){
      if( rc==SQLITE_NOMEM ) sqlite3OomFault(db);
      sqlite3_result_error(context, zErr, -1);
      sqlite3_free(zErr);
      return;
    }
    assert( pVfs );
    flags |= SQLITE_OPEN_MAIN_DB;
    rc = sqlite3BtreeOpen(pVfs, zPath, db, &pNew->pBt, 0, flags);
    db->nDb++;
    pNew->zDbSName = sqlite3DbStrDup(db, zName);
  }
  db->noSharedCache = 0;
  if( rc==SQLITE_CONSTRAINT ){
    rc = SQLITE_ERROR;
    zErrDyn = sqlite3MPrintf(db, "database is already attached");
  }else if( rc==SQLITE_OK ){
    Pager *pPager;
    pNew->pSchema = sqlite3SchemaGet(db, pNew->pBt);
    if( !pNew->pSchema ){
      rc = SQLITE_NOMEM_BKPT;
    }else if( pNew->pSchema->file_format && pNew->pSchema->enc!=ENC(db) ){
      zErrDyn = sqlite3MPrintf(db,
        "attached databases must use the same text encoding as main database");
      rc = SQLITE_ERROR;
    }
    sqlite3BtreeEnter(pNew->pBt);
    pPager = sqlite3BtreePager(pNew->pBt);
    sqlite3PagerLockingMode(pPager, db->dfltLockMode);
    sqlite3BtreeSecureDelete(pNew->pBt,
                             sqlite3BtreeSecureDelete(db->aDb[0].pBt,-1) );
#ifndef SQLITE_OMIT_PAGER_PRAGMAS
    sqlite3BtreeSetPagerFlags(pNew->pBt,
                      PAGER_SYNCHRONOUS_FULL | (db->flags & PAGER_FLAGS_MASK));
#endif
    sqlite3BtreeLeave(pNew->pBt);
  }
  pNew->safety_level = SQLITE_DEFAULT_SYNCHRONOUS+1;
  if( rc==SQLITE_OK && pNew->zDbSName==0 ){
    rc = SQLITE_NOMEM_BKPT;
  }


#ifdef SQLITE_HAS_CODEC
  if( rc==SQLITE_OK ){
    extern int sqlite3CodecAttach(sqlite3*, int, const void*, int);
    extern void sqlite3CodecGetKey(sqlite3*, int, void**, int*);
    int nKey;
    char *zKey;
    int t = sqlite3_value_type(argv[2]);
    switch( t ){
      case SQLITE_INTEGER:
      case SQLITE_FLOAT:
        zErrDyn = sqlite3DbStrDup(db, "Invalid key value");
        rc = SQLITE_ERROR;
        break;

      case SQLITE_TEXT:
      case SQLITE_BLOB:
        nKey = sqlite3_value_bytes(argv[2]);
        zKey = (char *)sqlite3_value_blob(argv[2]);
        rc = sqlite3CodecAttach(db, db->nDb-1, zKey, nKey);
        break;

      case SQLITE_NULL:
         
        if( sqlite3CodecQueryParameters(db, zName, zPath)==0 ){
          sqlite3CodecGetKey(db, 0, (void**)&zKey, &nKey);
          if( nKey || sqlite3BtreeGetOptimalReserve(db->aDb[0].pBt)>0 ){
            rc = sqlite3CodecAttach(db, db->nDb-1, zKey, nKey);
          }
        }
        break;
    }
  }
#endif
  sqlite3_free( zPath );

   
  if( rc==SQLITE_OK ){
    sqlite3BtreeEnterAll(db);
    db->init.iDb = 0;
    db->mDbFlags &= ~(DBFLAG_SchemaKnownOk);
    rc = sqlite3Init(db, &zErrDyn);
    sqlite3BtreeLeaveAll(db);
    assert( zErrDyn==0 || rc!=SQLITE_OK );
  }
#ifdef SQLITE_USER_AUTHENTICATION
  if( rc==SQLITE_OK ){
    u8 newAuth = 0;
    rc = sqlite3UserAuthCheckLogin(db, zName, &newAuth);
    if( newAuth<db->auth.authLevel ){
      rc = SQLITE_AUTH_USER;
    }
  }
#endif
  if( rc ){
    if( !REOPEN_AS_MEMDB(db) ){
      int iDb = db->nDb - 1;
      assert( iDb>=2 );
      if( db->aDb[iDb].pBt ){
        sqlite3BtreeClose(db->aDb[iDb].pBt);
        db->aDb[iDb].pBt = 0;
        db->aDb[iDb].pSchema = 0;
      }
      sqlite3ResetAllSchemasOfConnection(db);
      db->nDb = iDb;
      if( rc==SQLITE_NOMEM || rc==SQLITE_IOERR_NOMEM ){
        sqlite3OomFault(db);
        sqlite3DbFree(db, zErrDyn);
        zErrDyn = sqlite3MPrintf(db, "out of memory");
      }else if( zErrDyn==0 ){
        zErrDyn = sqlite3MPrintf(db, "unable to open database: %s", zFile);
      }
    }
    goto attach_error;
  }

  return;

attach_error:
   
  if( zErrDyn ){
    sqlite3_result_error(context, zErrDyn, -1);
    sqlite3DbFree(db, zErrDyn);
  }
  if( rc ) sqlite3_result_error_code(context, rc);
}
