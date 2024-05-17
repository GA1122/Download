bool OmniboxEditModel::IsSpaceCharForAcceptingKeyword(wchar_t c) {
  switch (c) {
    case 0x0020:   
    case 0x3000:   
      return true;
    default:
      return false;
  }
}
