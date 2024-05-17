    TestingAutomationProvider::GetCreditCardFieldToStringMap() {
  std::map<AutofillFieldType, std::string> credit_card_type_to_string;
  credit_card_type_to_string[CREDIT_CARD_NAME] = "CREDIT_CARD_NAME";
  credit_card_type_to_string[CREDIT_CARD_NUMBER] = "CREDIT_CARD_NUMBER";
  credit_card_type_to_string[CREDIT_CARD_EXP_MONTH] = "CREDIT_CARD_EXP_MONTH";
  credit_card_type_to_string[CREDIT_CARD_EXP_4_DIGIT_YEAR] =
      "CREDIT_CARD_EXP_4_DIGIT_YEAR";
  return credit_card_type_to_string;
}
