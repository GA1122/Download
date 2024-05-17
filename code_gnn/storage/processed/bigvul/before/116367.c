void RunTransactionTest(net::HttpCache* cache,
                        const MockTransaction& trans_info) {
  RunTransactionTestWithLog(cache, trans_info, net::BoundNetLog());
}
