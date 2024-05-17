ChromotingHost* ChromotingHost::Create(ChromotingHostContext* context,
                                       MutableHostConfig* config,
                                       DesktopEnvironment* environment,
                                       AccessVerifier* access_verifier,
                                       bool allow_nat_traversal) {
  return new ChromotingHost(context, config, environment, access_verifier,
                            allow_nat_traversal);
}
