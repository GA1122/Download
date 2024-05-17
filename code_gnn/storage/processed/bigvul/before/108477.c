 void QuotaManager::DidDatabaseWork(bool success) {
   db_disabled_ = !success;
 }
