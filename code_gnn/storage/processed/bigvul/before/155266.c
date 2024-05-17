void SetClientInformation(ExceptionInformation* exception,
                          SanitizationInformation* sanitization,
                          ClientInformation* info) {
  info->exception_information_address =
      FromPointerCast<decltype(info->exception_information_address)>(exception);

  info->sanitization_information_address =
      FromPointerCast<decltype(info->sanitization_information_address)>(
          sanitization);
}
