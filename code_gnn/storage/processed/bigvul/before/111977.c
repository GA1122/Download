void SyncTest::AddTestSwitches(CommandLine* cl) {
  if (!cl->HasSwitch(switches::kSyncNotificationMethod))
    cl->AppendSwitchASCII(switches::kSyncNotificationMethod, "p2p");

  if (!cl->HasSwitch(switches::kDisableBackgroundNetworking))
     cl->AppendSwitch(switches::kDisableBackgroundNetworking);
 }
