void AutoFillManager::FillCreditCardFormField(const CreditCard* credit_card,
                                              AutoFillType type,
                                              webkit_glue::FormField* field) {
  DCHECK(credit_card);
  DCHECK(type.group() == AutoFillType::CREDIT_CARD);
  DCHECK(field);

  if (field->form_control_type() == ASCIIToUTF16("select-one"))
    autofill::FillSelectControl(credit_card, type, field);
  else
    field->set_value(credit_card->GetFieldText(type));
}
