Document* Document::CreateForTest() {
  return MakeGarbageCollected<Document>(DocumentInit::Create());
}
