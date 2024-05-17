  UIDNAWrapper() {
    UErrorCode err = U_ZERO_ERROR;
    value = uidna_openUTS46(UIDNA_CHECK_BIDI, &err);
    if (U_FAILURE(err))
      value = NULL;
  }
