void HTMLFrameOwnerElement::UpdateContainerPolicy(Vector<String>* messages,
                                                  bool* old_syntax) {
  container_policy_ = ConstructContainerPolicy(messages, old_syntax);
  if (ContentFrame()) {
    GetDocument().GetFrame()->Client()->DidChangeFramePolicy(
        ContentFrame(), sandbox_flags_, container_policy_);
  }
}
