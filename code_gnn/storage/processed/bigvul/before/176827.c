std::u16string utf8ToUtf16(const StringPiece& utf8) {
 ssize_t utf16Length = utf8_to_utf16_length(reinterpret_cast<const uint8_t*>(utf8.data()),
            utf8.length());
 if (utf16Length <= 0) {
 return {};
 }

    std::u16string utf16;
    utf16.resize(utf16Length);
    utf8_to_utf16(reinterpret_cast<const uint8_t*>(utf8.data()), utf8.length(), &*utf16.begin());
 return utf16;
}
