  void DisableCreditCardAutofill() {
    scoped_feature_list_.InitAndEnableFeature(
        kAutofillCreditCardAblationExperiment);
  }
