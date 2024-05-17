void LiveSyncTest::TriggerMigrationDoneError(
    const syncable::ModelTypeSet& model_types) {
  ASSERT_TRUE(ServerSupportsErrorTriggering());
  std::string path = "chromiumsync/migrate";
  char joiner = '?';
  for (syncable::ModelTypeSet::const_iterator it = model_types.begin();
       it != model_types.end(); ++it) {
    path.append(base::StringPrintf("%ctype=%d", joiner,
        syncable::GetExtensionFieldNumberFromModelType(*it)));
    joiner = '&';
  }
  ui_test_utils::NavigateToURL(browser(), sync_server_.GetURL(path));
  ASSERT_EQ(ASCIIToUTF16("Migration: 200"),
            browser()->GetSelectedTabContents()->GetTitle());
}
