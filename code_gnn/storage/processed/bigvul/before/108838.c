static void ConstructFrameTree(WebKit::WebFrame* frame,
                               WebKit::WebFrame* exclude_frame_subtree,
                               base::DictionaryValue* dict) {
  dict->SetString(content::kFrameTreeNodeNameKey,
                  UTF16ToUTF8(frame->assignedName()).c_str());
  dict->SetInteger(content::kFrameTreeNodeIdKey, frame->identifier());

  WebFrame* child = frame->firstChild();
  ListValue* children = new ListValue();
  for (; child; child = child->nextSibling()) {
    if (child == exclude_frame_subtree)
      continue;

    base::DictionaryValue* d = new base::DictionaryValue();
    ConstructFrameTree(child, exclude_frame_subtree, d);
    children->Append(d);
  }
  if (children->GetSize() > 0)
    dict->Set(content::kFrameTreeNodeSubtreeKey, children);
}
