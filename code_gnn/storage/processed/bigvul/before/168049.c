PopupType AutofillManager::GetPopupType(const FormData& form,
                                        const FormFieldData& field) {
  const AutofillField* autofill_field = GetAutofillField(form, field);
  if (!autofill_field)
    return PopupType::kUnspecified;

  switch (autofill_field->Type().group()) {
    case NO_GROUP:
    case PASSWORD_FIELD:
    case TRANSACTION:
    case USERNAME_FIELD:
    case UNFILLABLE:
      return PopupType::kUnspecified;

    case CREDIT_CARD:
      return PopupType::kCreditCards;

    case ADDRESS_HOME:
    case ADDRESS_BILLING:
      return PopupType::kAddresses;

    case NAME:
    case NAME_BILLING:
    case EMAIL:
    case COMPANY:
    case PHONE_HOME:
    case PHONE_BILLING:
      return FormHasAddressField(form) ? PopupType::kAddresses
                                       : PopupType::kPersonalInformation;

    default:
      NOTREACHED();
  }
}
