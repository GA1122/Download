bool LiveSyncTest::SetUpLocalPythonTestServer() {
  EXPECT_TRUE(sync_server_.Start())
      << "Could not launch local python test server.";

  CommandLine* cl = CommandLine::ForCurrentProcess();
  std::string sync_service_url = sync_server_.GetURL("chromiumsync").spec();
  cl->AppendSwitchASCII(switches::kSyncServiceURL, sync_service_url);
  VLOG(1) << "Started local python test server at " << sync_service_url;

  int xmpp_port = 0;
  if (!sync_server_.server_data().GetInteger("xmpp_port", &xmpp_port)) {
    LOG(ERROR) << "Could not find valid xmpp_port value";
    return false;
  }
  if ((xmpp_port <= 0) || (xmpp_port > kuint16max)) {
    LOG(ERROR) << "Invalid xmpp port: " << xmpp_port;
    return false;
  }

  net::HostPortPair xmpp_host_port_pair(sync_server_.host_port_pair());
  xmpp_host_port_pair.set_port(xmpp_port);
  xmpp_port_.reset(new net::ScopedPortException(xmpp_port));

  if (!cl->HasSwitch(switches::kSyncNotificationHost)) {
    cl->AppendSwitchASCII(switches::kSyncNotificationHost,
                          xmpp_host_port_pair.ToString());
    cl->AppendSwitch(switches::kSyncAllowInsecureXmppConnection);
  }

  return true;
}
