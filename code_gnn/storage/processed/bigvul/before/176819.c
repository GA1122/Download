bool isJavaClassName(const StringPiece16& str) {
 size_t pieces = 0;
 for (const StringPiece16& piece : tokenize(str, u'.')) {
        pieces++;
 if (piece.empty()) {
 return false;
 }

 if (piece.data()[0] == u'$' || piece.data()[piece.size() - 1] == u'$') {
 return false;
 }

 if (findNonAlphaNumericAndNotInSet(piece, u"$_") != piece.end()) {
 return false;
 }
 }
 return pieces >= 2;
}
