 void SplitString(const string16& str,
                  char16 c,
                  std::vector<string16>* r) {
   DCHECK(CBU16_IS_SINGLE(c));
   SplitStringT(str, c, true, r);
 }
