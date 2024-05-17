void UsbSetInterfaceAlternateSettingFunction::OnComplete(bool success) {
  if (success) {
    Respond(NoArguments());
  } else {
    Respond(Error(kErrorCannotSetInterfaceAlternateSetting));
  }
}