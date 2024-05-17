base::string16 AutofillDialogViews::GetCvc() {
  return GroupForSection(GetCreditCardSection())->suggested_info->
      textfield()->GetText();
}
