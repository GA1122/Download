 void InputMethodIBus::ProcessUnfilteredFabricatedKeyPressEvent(
     EventType type,
     KeyboardCode key_code,
     int flags,
     guint32 ibus_keyval) {
   TextInputClient* client = GetTextInputClient();
   DispatchFabricatedKeyEventPostIME(type, key_code, flags);
 
   if (client != GetTextInputClient())
     return;
 
   if (character_composer_.FilterKeyPress(ibus_keyval, flags)) {
      string16 composed = character_composer_.composed_character();
      if (!composed.empty()) {
        client = GetTextInputClient();
      if (client)
        client->InsertText(composed);
//       if (client) {
//          
//          
//          
//         if (composed.size() == 1)
//           client->InsertChar(composed[0], flags);
//         else
//           client->InsertText(composed);
//       }
      }
      return;
    }
 
   client = GetTextInputClient();
   const uint16 ch = ui::GetCharacterFromKeyCode(key_code, flags);
   if (client && ch)
     client->InsertChar(ch, flags);
 }