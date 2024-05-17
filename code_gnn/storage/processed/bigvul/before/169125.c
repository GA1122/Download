static bool TokenExitsForeignContent(const CompactHTMLToken& token) {
  const String& tag_name = token.Data();
  return ThreadSafeMatch(tag_name, bTag) || ThreadSafeMatch(tag_name, bigTag) ||
         ThreadSafeMatch(tag_name, blockquoteTag) ||
         ThreadSafeMatch(tag_name, bodyTag) ||
         ThreadSafeMatch(tag_name, brTag) ||
         ThreadSafeMatch(tag_name, centerTag) ||
         ThreadSafeMatch(tag_name, codeTag) ||
         ThreadSafeMatch(tag_name, ddTag) ||
         ThreadSafeMatch(tag_name, divTag) ||
         ThreadSafeMatch(tag_name, dlTag) || ThreadSafeMatch(tag_name, dtTag) ||
         ThreadSafeMatch(tag_name, emTag) ||
         ThreadSafeMatch(tag_name, embedTag) ||
         ThreadSafeMatch(tag_name, h1Tag) || ThreadSafeMatch(tag_name, h2Tag) ||
         ThreadSafeMatch(tag_name, h3Tag) || ThreadSafeMatch(tag_name, h4Tag) ||
         ThreadSafeMatch(tag_name, h5Tag) || ThreadSafeMatch(tag_name, h6Tag) ||
         ThreadSafeMatch(tag_name, headTag) ||
         ThreadSafeMatch(tag_name, hrTag) || ThreadSafeMatch(tag_name, iTag) ||
         ThreadSafeMatch(tag_name, imgTag) ||
         ThreadSafeMatch(tag_name, liTag) ||
         ThreadSafeMatch(tag_name, listingTag) ||
         ThreadSafeMatch(tag_name, menuTag) ||
         ThreadSafeMatch(tag_name, metaTag) ||
         ThreadSafeMatch(tag_name, nobrTag) ||
         ThreadSafeMatch(tag_name, olTag) || ThreadSafeMatch(tag_name, pTag) ||
         ThreadSafeMatch(tag_name, preTag) ||
         ThreadSafeMatch(tag_name, rubyTag) ||
         ThreadSafeMatch(tag_name, sTag) ||
         ThreadSafeMatch(tag_name, smallTag) ||
         ThreadSafeMatch(tag_name, spanTag) ||
         ThreadSafeMatch(tag_name, strongTag) ||
         ThreadSafeMatch(tag_name, strikeTag) ||
         ThreadSafeMatch(tag_name, subTag) ||
         ThreadSafeMatch(tag_name, supTag) ||
         ThreadSafeMatch(tag_name, tableTag) ||
         ThreadSafeMatch(tag_name, ttTag) || ThreadSafeMatch(tag_name, uTag) ||
         ThreadSafeMatch(tag_name, ulTag) ||
         ThreadSafeMatch(tag_name, varTag) ||
         (ThreadSafeMatch(tag_name, fontTag) &&
          (token.GetAttributeItem(colorAttr) ||
           token.GetAttributeItem(faceAttr) ||
            token.GetAttributeItem(sizeAttr)));
 }
