std::wstring GetExtensionWithoutLeadingDot(const std::wstring& extension) {
  DCHECK(extension.empty() || extension[0] == L'.');
   return extension.empty() ? extension : extension.substr(1);
 }
