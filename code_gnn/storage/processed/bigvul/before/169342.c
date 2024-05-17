void SimulateNetworkServiceCrash() {
  CHECK(base::FeatureList::IsEnabled(network::features::kNetworkService));
  CHECK(!IsNetworkServiceRunningInProcess())
      << "Can't crash the network service if it's running in-process!";
  network::mojom::NetworkServiceTestPtr network_service_test;
  ServiceManagerConnection::GetForProcess()->GetConnector()->BindInterface(
      mojom::kNetworkServiceName, &network_service_test);

  base::RunLoop run_loop(base::RunLoop::Type::kNestableTasksAllowed);
  network_service_test.set_connection_error_handler(run_loop.QuitClosure());

  network_service_test->SimulateCrash();
  run_loop.Run();

  FlushNetworkServiceInstanceForTesting();
}
