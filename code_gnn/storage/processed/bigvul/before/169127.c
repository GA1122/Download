 static bool TokenExitsMath(const CompactHTMLToken& token) {
  const String& tag_name = token.Data();
  return ThreadSafeMatch(tag_name, MathMLNames::miTag) ||
         ThreadSafeMatch(tag_name, MathMLNames::moTag) ||
         ThreadSafeMatch(tag_name, MathMLNames::mnTag) ||
         ThreadSafeMatch(tag_name, MathMLNames::msTag) ||
         ThreadSafeMatch(tag_name, MathMLNames::mtextTag);
}
