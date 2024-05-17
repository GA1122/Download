void InputMethodIBus::DeleteSurroundingText(int32 offset, uint32 length) {
  if (!composition_.text.empty())
    return;   
  if (offset < 0 && static_cast<uint32>(-1 * offset) != length)
    return;   
  if (GetTextInputClient())
    GetTextInputClient()->ExtendSelectionAndDelete(length, 0U);
}
