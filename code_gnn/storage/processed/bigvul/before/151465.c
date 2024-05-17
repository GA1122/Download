  FrameFetchContext* CreateChildFrame() {
    child_client = StubLocalFrameClientWithParent::Create(document->GetFrame());
    child_frame = LocalFrame::Create(
        child_client.Get(), *document->GetFrame()->GetPage(), owner.Get());
    child_frame->SetView(
        LocalFrameView::Create(*child_frame, IntSize(500, 500)));
    child_frame->Init();
    child_document = child_frame->GetDocument();
    FrameFetchContext* child_fetch_context = static_cast<FrameFetchContext*>(
        &child_frame->Loader().GetDocumentLoader()->Fetcher()->Context());
    FrameFetchContext::ProvideDocumentToContext(*child_fetch_context,
                                                child_document.Get());
    return child_fetch_context;
  }
