  void SetLegalMessage(const std::string& message_json,
                       bool should_cvc_be_requested = false) {
    std::unique_ptr<base::Value> value(base::JSONReader::Read(message_json));
    ASSERT_TRUE(value);
    base::DictionaryValue* dictionary;
    ASSERT_TRUE(value->GetAsDictionary(&dictionary));
    std::unique_ptr<base::DictionaryValue> legal_message =
        dictionary->CreateDeepCopy();
    controller()->ShowBubbleForUpload(CreditCard(), std::move(legal_message),
                                      should_cvc_be_requested,
                                      base::Bind(&SaveCardCallback));
  }
