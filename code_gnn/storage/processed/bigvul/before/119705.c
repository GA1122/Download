void ResetScreenHandler::DeclareLocalizedValues(
    LocalizedValuesBuilder* builder) {
  builder->Add("resetScreenTitle", IDS_RESET_SCREEN_TITLE);
  builder->Add("cancelButton", IDS_CANCEL);

  builder->Add("resetWarningDataDetails",
               IDS_RESET_SCREEN_WARNING_DETAILS_DATA);
  builder->Add("resetRestartMessage", IDS_RESET_SCREEN_RESTART_MSG);
  builder->AddF("resetRollbackOption",
                IDS_RESET_SCREEN_ROLLBACK_OPTION,
                IDS_SHORT_PRODUCT_NAME);
  builder->AddF("resetRevertPromise",
               IDS_RESET_SCREEN_PREPARING_REVERT_PROMISE,
               IDS_SHORT_PRODUCT_NAME);
  builder->AddF("resetRevertSpinnerMessage",
                IDS_RESET_SCREEN_PREPARING_REVERT_SPINNER_MESSAGE,
                IDS_SHORT_PRODUCT_NAME);

  builder->Add("resetButtonReset", IDS_RESET_SCREEN_RESET);
  builder->Add("resetButtonRelaunch", IDS_RELAUNCH_BUTTON);
  builder->Add("resetButtonPowerwash", IDS_RESET_SCREEN_POWERWASH);

  builder->AddF(
      "resetAndRollbackWarningTextConfirmational",
      IDS_RESET_SCREEN_CONFIRMATION_WARNING_POWERWASH_AND_ROLLBACK_MSG,
      IDS_SHORT_PRODUCT_NAME);
  builder->AddF("resetWarningTextConfirmational",
                IDS_RESET_SCREEN_CONFIRMATION_WARNING_POWERWASH_MSG,
                IDS_SHORT_PRODUCT_NAME);
  builder->AddF("resetWarningTextInitial",
                IDS_RESET_SCREEN_WARNING_MSG,
                IDS_SHORT_PRODUCT_NAME);

  builder->AddF(
      "resetAndRollbackWarningDetailsConfirmational",
      IDS_RESET_SCREEN_CONFIRMATION_WARNING_ROLLBACK_DETAILS,
      IDS_SHORT_PRODUCT_NAME);
  builder->AddF("resetWarningDetailsConfirmational",
                IDS_RESET_SCREEN_CONFIRMATION_WARNING_DETAILS,
                IDS_SHORT_PRODUCT_NAME);
  builder->AddF("resetWarningDetailsInitial",
                IDS_RESET_SCREEN_WARNING_DETAILS,
                IDS_SHORT_PRODUCT_NAME);
}
