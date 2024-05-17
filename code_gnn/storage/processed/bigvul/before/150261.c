mojom::KeypressResponseForRulebasedPtr GenerateKeypressResponseForRulebased(
    rulebased::ProcessKeyResult& process_key_result) {
  mojom::KeypressResponseForRulebasedPtr keypress_response =
      mojom::KeypressResponseForRulebased::New();
  keypress_response->result = process_key_result.key_handled;
  if (!process_key_result.commit_text.empty()) {
    std::string commit_text;
    base::EscapeJSONString(process_key_result.commit_text, false, &commit_text);
    keypress_response->operations.push_back(mojom::OperationForRulebased::New(
        mojom::OperationMethodForRulebased::COMMIT_TEXT, commit_text));
  }
  if (!process_key_result.composition_text.empty() ||
      (process_key_result.key_handled &&
       process_key_result.commit_text.empty())) {
    std::string composition_text;
    base::EscapeJSONString(process_key_result.composition_text, false,
                           &composition_text);
    keypress_response->operations.push_back(mojom::OperationForRulebased::New(
        mojom::OperationMethodForRulebased::SET_COMPOSITION, composition_text));
  }
  return keypress_response;
}
