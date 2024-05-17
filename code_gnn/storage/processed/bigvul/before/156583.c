  FactoryImpl(SyncSessionsClient* sessions_client,
              const SessionStore::RestoredForeignTabCallback&
                  restored_foreign_tab_callback)
      : sessions_client_(sessions_client),
        restored_foreign_tab_callback_(restored_foreign_tab_callback) {
    DCHECK(sessions_client);
  }
