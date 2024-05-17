void TestProcessKeypressForRulebasedCallback(
    mojom::KeypressResponseForRulebased* res_out,
    mojom::KeypressResponseForRulebasedPtr response) {
  res_out->result = response->result;
  res_out->operations = std::vector<mojom::OperationForRulebasedPtr>(0);
  for (int i = 0; i < (int)response->operations.size(); i++) {
    res_out->operations.push_back(std::move(response->operations[i]));
  }
}
