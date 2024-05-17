static void writeImageNodeToPasteboard(Pasteboard* pasteboard,
                                       Node* node,
                                       const String& title) {
  DCHECK(pasteboard);
  DCHECK(node);

  RefPtr<Image> image = imageFromNode(*node);
  if (!image.get())
    return;

  AtomicString urlString;
  if (isHTMLImageElement(*node) || isHTMLInputElement(*node))
    urlString = toHTMLElement(node)->getAttribute(srcAttr);
  else if (isSVGImageElement(*node))
    urlString = toSVGElement(node)->imageSourceURL();
  else if (isHTMLEmbedElement(*node) || isHTMLObjectElement(*node) ||
           isHTMLCanvasElement(*node))
    urlString = toHTMLElement(node)->imageSourceURL();
  KURL url = urlString.isEmpty()
                 ? KURL()
                 : node->document().completeURL(
                       stripLeadingAndTrailingHTMLSpaces(urlString));

  pasteboard->writeImage(image.get(), url, title);
}
