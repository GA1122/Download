void UsbFindDevicesFunction::OpenComplete() {
  Respond(OneArgument(result_.release()));
}
