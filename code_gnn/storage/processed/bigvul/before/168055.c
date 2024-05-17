bool IsCreditCardExpirationType(ServerFieldType type) {
  return type == CREDIT_CARD_EXP_MONTH ||
         type == CREDIT_CARD_EXP_2_DIGIT_YEAR ||
         type == CREDIT_CARD_EXP_4_DIGIT_YEAR ||
         type == CREDIT_CARD_EXP_DATE_2_DIGIT_YEAR ||
         type == CREDIT_CARD_EXP_DATE_4_DIGIT_YEAR;
}
