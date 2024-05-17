static bool isListElement(Node* node) {
  return isHTMLUListElement(*node) || isHTMLOListElement(*node) ||
         isHTMLDListElement(*node);
}
