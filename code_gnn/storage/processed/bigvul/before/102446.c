string16 StripWWW(const string16& text) {
  const string16 www(ASCIIToUTF16("www."));
  return StartsWith(text, www, true) ? text.substr(www.length()) : text;
}
