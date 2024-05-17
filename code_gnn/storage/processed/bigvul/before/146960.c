  void DispatchEventsForPrintingOnAllFrames() {
    HeapVector<Member<Document>> documents;
    for (Frame* current_frame = GetFrame(); current_frame;
         current_frame = current_frame->Tree().TraverseNext(GetFrame())) {
      if (current_frame->IsLocalFrame())
        documents.push_back(ToLocalFrame(current_frame)->GetDocument());
    }

    for (auto& doc : documents)
      doc->DispatchEventsForPrinting();
  }
