  void SetFilterPolicy(WebDocumentSubresourceFilter::LoadPolicy policy) {
    document->Loader()->SetSubresourceFilter(SubresourceFilter::Create(
        *document, WTF::MakeUnique<FixedPolicySubresourceFilter>(
                       policy, &filtered_load_callback_counter_)));
  }
