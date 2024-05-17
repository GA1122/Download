void ScriptResource::DestroyDecodedDataForFailedRevalidation() {
  source_text_ = AtomicString();
   SetDecodedSize(0);
 }
