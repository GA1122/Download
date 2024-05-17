base::string16 StripWWW(const base::string16& text) {
  const base::string16 www(base::ASCIIToUTF16("www."));
  return base::StartsWith(text, www, base::CompareCase::SENSITIVE)
      ? text.substr(www.length()) : text;
}
