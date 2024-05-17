static void WriteImageNodeToPasteboard(Pasteboard* pasteboard,
                                       Node* node,
                                       const String& title) {
  DCHECK(pasteboard);
  DCHECK(node);

  RefPtr<Image> image = ImageFromNode(*node);
  if (!image.Get())
    return;

  AtomicString url_string;
  if (isHTMLImageElement(*node) || isHTMLInputElement(*node))
    url_string = ToHTMLElement(node)->getAttribute(srcAttr);
  else if (isSVGImageElement(*node))
    url_string = ToSVGElement(node)->ImageSourceURL();
  else if (isHTMLEmbedElement(*node) || isHTMLObjectElement(*node) ||
           isHTMLCanvasElement(*node))
    url_string = ToHTMLElement(node)->ImageSourceURL();
  KURL url = url_string.IsEmpty()
                 ? KURL()
                 : node->GetDocument().CompleteURL(
                       StripLeadingAndTrailingHTMLSpaces(url_string));

  pasteboard->WriteImage(image.Get(), url, title);
}
