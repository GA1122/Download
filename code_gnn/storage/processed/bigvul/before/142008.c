 bool IsAutofillSaveCardDialogUnlabeledExpirationDateEnabled() {
   return base::FeatureList::IsEnabled(
       kAutofillSaveCardDialogUnlabeledExpirationDate);
}
