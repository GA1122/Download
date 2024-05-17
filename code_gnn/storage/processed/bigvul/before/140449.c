void HttpNetworkTransactionTest::ConnectStatusHelper(
    const MockRead& status) {
  ConnectStatusHelperWithExpectedStatus(
      status, ERR_TUNNEL_CONNECTION_FAILED);
}
