void LiveSyncTest::SetUpCommandLine(CommandLine* cl) {
  if (!cl->HasSwitch(switches::kSyncNotificationMethod))
    cl->AppendSwitchASCII(switches::kSyncNotificationMethod, "p2p");

  if (!cl->HasSwitch(switches::kEnableSyncSessions))
    cl->AppendSwitch(switches::kEnableSyncSessions);

  if (!cl->HasSwitch(switches::kEnableSyncTypedUrls))
    cl->AppendSwitch(switches::kEnableSyncTypedUrls);

  if (!cl->HasSwitch(switches::kDisableBackgroundNetworking))
    cl->AppendSwitch(switches::kDisableBackgroundNetworking);
}
