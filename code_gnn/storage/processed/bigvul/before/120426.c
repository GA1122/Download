  void TryBasicFormFill() {
    FocusFirstNameField();

    SendKeyToPageAndWait(ui::VKEY_M);

    SendKeyToPopupAndWait(ui::VKEY_DOWN);

    ExpectFieldValue("firstname", "M");
    ExpectFieldValue("lastname", std::string());
    ExpectFieldValue("address1", std::string());
    ExpectFieldValue("address2", std::string());
    ExpectFieldValue("city", std::string());
    ExpectFieldValue("state", std::string());
    ExpectFieldValue("zip", std::string());
    ExpectFieldValue("country", std::string());
    ExpectFieldValue("phone", std::string());

    SendKeyToPopupAndWait(ui::VKEY_RETURN);

    ExpectFilledTestForm();
  }
