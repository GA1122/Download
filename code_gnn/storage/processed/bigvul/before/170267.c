  void SetShouldRequireCTForTesting(bool* required) {
    if (base::FeatureList::IsEnabled(network::features::kNetworkService)) {
      network::mojom::NetworkServiceTestPtr network_service_test;
      content::ServiceManagerConnection::GetForProcess()
          ->GetConnector()
          ->BindInterface(content::mojom::kNetworkServiceName,
                          &network_service_test);
      network::mojom::NetworkServiceTest::ShouldRequireCT required_ct;
      if (!required) {
        required_ct =
            network::mojom::NetworkServiceTest::ShouldRequireCT::RESET;
      } else {
        required_ct =
            *required
                ? network::mojom::NetworkServiceTest::ShouldRequireCT::REQUIRE
                : network::mojom::NetworkServiceTest::ShouldRequireCT::
                      DONT_REQUIRE;
      }

      mojo::ScopedAllowSyncCallForTesting allow_sync_call;
      network_service_test->SetShouldRequireCT(required_ct);
      return;
    }

    base::PostTaskWithTraits(
        FROM_HERE, {BrowserThread::IO},
        base::BindOnce(
            &net::TransportSecurityState::SetShouldRequireCTForTesting,
            required));
  }
