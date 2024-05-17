void doMergeFormDict(Dict *srcFormDict, Dict *mergeFormDict, int numOffset) {
  Object srcFields, mergeFields;

  srcFormDict->lookup("Fields", &srcFields);
  mergeFormDict->lookup("Fields", &mergeFields);
  if (srcFields.isArray() && mergeFields.isArray()) {
    for (int i = 0; i < mergeFields.arrayGetLength(); i++) {
      Object value;
      Object *newValue = new Object();
      mergeFields.arrayGetNF(i, &value);
      newValue->initRef(value.getRef().num + numOffset, value.getRef().gen);
      srcFields.arrayAdd(newValue);
      value.free();
      delete newValue;
    }
  }
  srcFields.free();
  mergeFields.free();
}
