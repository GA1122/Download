bool extractResFilePathParts(const StringPiece16& path, StringPiece16* outPrefix,
 StringPiece16* outEntry, StringPiece16* outSuffix) {
 if (!stringStartsWith<char16_t>(path, u"res/")) {
 return false;
 }

 StringPiece16::const_iterator lastOccurence = path.end();
 for (auto iter = path.begin() + StringPiece16(u"res/").size(); iter != path.end(); ++iter) {
 if (*iter == u'/') {
            lastOccurence = iter;
 }
 }

 if (lastOccurence == path.end()) {
 return false;
 }

 auto iter = std::find(lastOccurence, path.end(), u'.');
 *outSuffix = StringPiece16(iter, path.end() - iter);
 *outEntry = StringPiece16(lastOccurence + 1, iter - lastOccurence - 1);
 *outPrefix = StringPiece16(path.begin(), lastOccurence - path.begin() + 1);
 return true;
}
