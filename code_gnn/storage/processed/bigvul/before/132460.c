void UsbSetConfigurationFunction::OnComplete(bool success) {
  if (success) {
    Respond(NoArguments());
  } else {
    Respond(Error(kErrorCannotSetConfiguration));
  }
}
