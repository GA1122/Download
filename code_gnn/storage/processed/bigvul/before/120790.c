void BluetoothAdapterChromeOS::SetDefaultAdapterName() {
  std::string board = base::SysInfo::GetLsbReleaseBoard();
  std::string alias;
  if (board.substr(0, 6) == "stumpy") {
    alias = "Chromebox";
  } else if (board.substr(0, 4) == "link") {
    alias = "Chromebook Pixel";
  } else {
    alias = "Chromebook";
  }

  SetName(alias, base::Bind(&base::DoNothing), base::Bind(&base::DoNothing));
}
