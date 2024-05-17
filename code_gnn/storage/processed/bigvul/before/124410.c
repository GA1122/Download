UScriptCode GetScriptForWebSettings(UScriptCode scriptCode) {
  switch (scriptCode) {
  case USCRIPT_HIRAGANA:
  case USCRIPT_KATAKANA:
  case USCRIPT_JAPANESE:
    return USCRIPT_KATAKANA_OR_HIRAGANA;
  case USCRIPT_KOREAN:
    return USCRIPT_HANGUL;
  default:
    return scriptCode;
  }
}
