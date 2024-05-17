void IOThread::InitializeNetworkOptions(const CommandLine& command_line) {
  if (command_line.HasSwitch(switches::kEnableFileCookies)) {
    net::CookieMonster::EnableFileScheme();
  }

  if (is_spdy_disabled_by_policy_) {
    base::FieldTrial* trial = base::FieldTrialList::Find(kSpdyFieldTrialName);
    if (trial)
      trial->Disable();
  } else {
    std::string spdy_trial_group =
        base::FieldTrialList::FindFullName(kSpdyFieldTrialName);

    if (command_line.HasSwitch(switches::kEnableIPPooling))
      globals_->enable_spdy_ip_pooling.set(true);

    if (command_line.HasSwitch(switches::kDisableIPPooling))
      globals_->enable_spdy_ip_pooling.set(false);

    if (command_line.HasSwitch(switches::kEnableWebSocketOverSpdy)) {
      net::WebSocketJob::set_websocket_over_spdy_enabled(true);
    }

    if (command_line.HasSwitch(switches::kMaxSpdyConcurrentStreams)) {
      globals_->max_spdy_concurrent_streams_limit.set(
          GetSwitchValueAsInt(command_line,
                              switches::kMaxSpdyConcurrentStreams));
    }
    if (command_line.HasSwitch(switches::kTrustedSpdyProxy)) {
      globals_->trusted_spdy_proxy.set(
          command_line.GetSwitchValueASCII(switches::kTrustedSpdyProxy));
    }
    if (command_line.HasSwitch(switches::kIgnoreUrlFetcherCertRequests))
      net::URLFetcher::SetIgnoreCertificateRequests(true);

    if (command_line.HasSwitch(switches::kUseSpdy)) {
      std::string spdy_mode =
          command_line.GetSwitchValueASCII(switches::kUseSpdy);
      EnableSpdy(spdy_mode);
    } else if (command_line.HasSwitch(switches::kEnableHttp2Draft04)) {
      net::HttpStreamFactory::EnableNpnHttp2Draft04();
    } else if (command_line.HasSwitch(switches::kEnableSpdy4a2)) {
      net::HttpStreamFactory::EnableNpnSpdy4a2();
    } else if (command_line.HasSwitch(switches::kDisableSpdy31)) {
      net::HttpStreamFactory::EnableNpnSpdy3();
    } else if (command_line.HasSwitch(switches::kEnableSpdy2)) {
      net::HttpStreamFactory::EnableNpnSpdy31WithSpdy2();
    } else if (command_line.HasSwitch(switches::kEnableNpnHttpOnly)) {
      net::HttpStreamFactory::EnableNpnHttpOnly();
    } else {
      if (spdy_trial_group == kSpdyFieldTrialDisabledGroupName &&
          !command_line.HasSwitch(switches::kEnableWebSocketOverSpdy)) {
         net::HttpStreamFactory::set_spdy_enabled(false);
      } else {
        net::HttpStreamFactory::EnableNpnSpdy31();
      }
    }
  }

  if (command_line.HasSwitch(switches::kEnableTcpFastOpen))
    net::SetTCPFastOpenEnabled(true);
}
