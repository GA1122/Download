  int DirectoryBackingStore::CreateTables() {
    VLOG(1) << "First run, creating tables";
   int result = ExecQuery(load_dbhandle_,
                          "CREATE TABLE share_version ("
                          "id VARCHAR(128) primary key, data INT)");
   if (result != SQLITE_DONE)
     return result;
   {
     sqlite_utils::SQLStatement statement;
     statement.prepare(load_dbhandle_, "INSERT INTO share_version VALUES(?, ?)");
     statement.bind_string(0, dir_name_);
     statement.bind_int(1, kCurrentDBVersion);
     result = statement.step();
   }
   if (result != SQLITE_DONE)
     return result;
 
   const bool kCreateAsTempShareInfo = false;
   result = CreateShareInfoTable(kCreateAsTempShareInfo);
   if (result != SQLITE_DONE)
     return result;
   {
     sqlite_utils::SQLStatement statement;
     statement.prepare(load_dbhandle_, "INSERT INTO share_info VALUES"
                                       "(?, "   
                                       "?, "    
                                       "?, "    
                                       "?, "    
                                       "?, "    
                                       "-2, "   
                                       "?, "    
                                       "?);");   
     statement.bind_string(0, dir_name_);                    
     statement.bind_string(1, dir_name_);                    
     statement.bind_string(2, "");                           
     statement.bind_string(3, SYNC_ENGINE_VERSION_STRING);   
     statement.bind_int(4, static_cast<int32>(time(0)));     
     statement.bind_string(5, GenerateCacheGUID());          
     statement.bind_blob(11, NULL, 0);                       
     result = statement.step();
   }
   if (result != SQLITE_DONE)
     return result;
 
   result = CreateModelsTable();
   if (result != SQLITE_DONE)
     return result;
   result = CreateMetasTable(false);
   if (result != SQLITE_DONE)
      return result;
    {
    const int64 now = browser_sync::TimeToProtoTime(base::Time::Now());
//     const int64 now = Now();
      sqlite_utils::SQLStatement statement;
      statement.prepare(load_dbhandle_,
                        "INSERT INTO metas "
                       "( id, metahandle, is_dir, ctime, mtime) "
                       "VALUES ( \"r\", 1, 1, ?, ?)");
     statement.bind_int64(0, now);
     statement.bind_int64(1, now);
     result = statement.step();
   }
   return result;
 }