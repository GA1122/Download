void ExpectDevicesAndThen(
    const std::set<std::string>& expected_guids,
    base::OnceClosure continuation,
    std::vector<device::mojom::UsbDeviceInfoPtr> results) {
  EXPECT_EQ(expected_guids.size(), results.size());
  std::set<std::string> actual_guids;
  for (size_t i = 0; i < results.size(); ++i)
    actual_guids.insert(results[i]->guid);
  EXPECT_EQ(expected_guids, actual_guids);
  std::move(continuation).Run();
}
