static int assert_pager_state(Pager *p){
  Pager *pPager = p;

   
  assert( p->eState==PAGER_OPEN
       || p->eState==PAGER_READER
       || p->eState==PAGER_WRITER_LOCKED
       || p->eState==PAGER_WRITER_CACHEMOD
       || p->eState==PAGER_WRITER_DBMOD
       || p->eState==PAGER_WRITER_FINISHED
       || p->eState==PAGER_ERROR
  );

   
  assert( p->tempFile==0 || p->eLock==EXCLUSIVE_LOCK );
  assert( p->tempFile==0 || pPager->changeCountDone );

   
  assert( p->journalMode==PAGER_JOURNALMODE_OFF || p->useJournal );
  assert( p->journalMode!=PAGER_JOURNALMODE_OFF || !isOpen(p->jfd) );

   
  if( MEMDB ){
    assert( !isOpen(p->fd) );
    assert( p->noSync );
    assert( p->journalMode==PAGER_JOURNALMODE_OFF
         || p->journalMode==PAGER_JOURNALMODE_MEMORY
    );
    assert( p->eState!=PAGER_ERROR && p->eState!=PAGER_OPEN );
    assert( pagerUseWal(p)==0 );
  }

   
  assert( pPager->changeCountDone==0 || pPager->eLock>=RESERVED_LOCK );
  assert( p->eLock!=PENDING_LOCK );

  switch( p->eState ){
    case PAGER_OPEN:
      assert( !MEMDB );
      assert( pPager->errCode==SQLITE_OK );
      assert( sqlite3PcacheRefCount(pPager->pPCache)==0 || pPager->tempFile );
      break;

    case PAGER_READER:
      assert( pPager->errCode==SQLITE_OK );
      assert( p->eLock!=UNKNOWN_LOCK );
      assert( p->eLock>=SHARED_LOCK );
      break;

    case PAGER_WRITER_LOCKED:
      assert( p->eLock!=UNKNOWN_LOCK );
      assert( pPager->errCode==SQLITE_OK );
      if( !pagerUseWal(pPager) ){
        assert( p->eLock>=RESERVED_LOCK );
      }
      assert( pPager->dbSize==pPager->dbOrigSize );
      assert( pPager->dbOrigSize==pPager->dbFileSize );
      assert( pPager->dbOrigSize==pPager->dbHintSize );
      assert( pPager->setMaster==0 );
      break;

    case PAGER_WRITER_CACHEMOD:
      assert( p->eLock!=UNKNOWN_LOCK );
      assert( pPager->errCode==SQLITE_OK );
      if( !pagerUseWal(pPager) ){
         
        assert( p->eLock>=RESERVED_LOCK );
        assert( isOpen(p->jfd)
             || p->journalMode==PAGER_JOURNALMODE_OFF
             || p->journalMode==PAGER_JOURNALMODE_WAL
        );
      }
      assert( pPager->dbOrigSize==pPager->dbFileSize );
      assert( pPager->dbOrigSize==pPager->dbHintSize );
      break;

    case PAGER_WRITER_DBMOD:
      assert( p->eLock==EXCLUSIVE_LOCK );
      assert( pPager->errCode==SQLITE_OK );
      assert( !pagerUseWal(pPager) );
      assert( p->eLock>=EXCLUSIVE_LOCK );
      assert( isOpen(p->jfd)
           || p->journalMode==PAGER_JOURNALMODE_OFF
           || p->journalMode==PAGER_JOURNALMODE_WAL
           || (sqlite3OsDeviceCharacteristics(p->fd)&SQLITE_IOCAP_BATCH_ATOMIC)
      );
      assert( pPager->dbOrigSize<=pPager->dbHintSize );
      break;

    case PAGER_WRITER_FINISHED:
      assert( p->eLock==EXCLUSIVE_LOCK );
      assert( pPager->errCode==SQLITE_OK );
      assert( !pagerUseWal(pPager) );
      assert( isOpen(p->jfd)
           || p->journalMode==PAGER_JOURNALMODE_OFF
           || p->journalMode==PAGER_JOURNALMODE_WAL
           || (sqlite3OsDeviceCharacteristics(p->fd)&SQLITE_IOCAP_BATCH_ATOMIC)
      );
      break;

    case PAGER_ERROR:
       
      assert( pPager->errCode!=SQLITE_OK );
      assert( sqlite3PcacheRefCount(pPager->pPCache)>0 || pPager->tempFile );
      break;
  }

  return 1;
}
