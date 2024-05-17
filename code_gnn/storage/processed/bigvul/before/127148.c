  void SubmitCreditCard(const char* name,
                        const char* number,
                        const char* exp_month,
                        const char* exp_year) {
    FormMap data;
    data["CREDIT_CARD_NAME"] = name;
    data["CREDIT_CARD_NUMBER"] = number;
    data["CREDIT_CARD_EXP_MONTH"] = exp_month;
    data["CREDIT_CARD_EXP_4_DIGIT_YEAR"] = exp_year;
    FillFormAndSubmit("autofill_creditcard_form.html", data);
  }
