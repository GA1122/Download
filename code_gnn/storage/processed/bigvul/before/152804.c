  Element* GetElementById(const AtomicString& id) {
    return MainFrame().GetFrame()->GetDocument()->getElementById(id);
  }
