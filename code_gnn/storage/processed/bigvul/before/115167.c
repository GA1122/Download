void BluetoothOptionsHandler::EnableChangeCallback(
    const ListValue* args) {
  bool bluetooth_enabled;
  args->GetBoolean(0, &bluetooth_enabled);
  base::FundamentalValue checked(bluetooth_enabled);
  web_ui_->CallJavascriptFunction(
      "options.SystemOptions.setBluetoothCheckboxState", checked);
}
