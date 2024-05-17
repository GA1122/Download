base::string16 TemplateURL::GenerateKeyword(const GURL& url) {
  DCHECK(url.is_valid());
  base::string16 keyword = url_formatter::StripWWW(
      url_formatter::IDNToUnicode(url.host()));
  return keyword.empty() ? base::ASCIIToUTF16("www") : keyword;
}
