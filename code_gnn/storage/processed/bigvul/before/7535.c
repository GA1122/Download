void doMergeNameDict(PDFDoc *doc, XRef *srcXRef, XRef *countRef, int oldRefNum, int newRefNum, Dict *srcNameDict, Dict *mergeNameDict, int numOffset) {
  for (int i = 0; i < mergeNameDict->getLength(); i++) {
    const char *key = mergeNameDict->getKey(i);
    Object mergeNameTree;
    Object srcNameTree;
    mergeNameDict->lookup(key, &mergeNameTree);
    srcNameDict->lookup(key, &srcNameTree);
    if (srcNameTree.isDict() && mergeNameTree.isDict()) {
      doMergeNameTree(doc, srcXRef, countRef, oldRefNum, newRefNum, srcNameTree.getDict(), mergeNameTree.getDict(), numOffset);
    } else if (srcNameTree.isNull() && mergeNameTree.isDict()) {
      Object *newNameTree = new Object();
      newNameTree->initDict(srcXRef);
      doMergeNameTree(doc, srcXRef, countRef, oldRefNum, newRefNum, newNameTree->getDict(), mergeNameTree.getDict(), numOffset);
      srcNameDict->add(copyString(key), newNameTree);
    }
    srcNameTree.free();
    mergeNameTree.free();
  }
}
