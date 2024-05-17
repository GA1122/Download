void RenderFrameImpl::SerializeAsMHTML(mojom::SerializeAsMHTMLParamsPtr params,
                                       SerializeAsMHTMLCallback callback) {
  TRACE_EVENT0("page-serialization", "RenderFrameImpl::SerializeAsMHTML");
  base::TimeTicks start_time = base::TimeTicks::Now();

  const WebString mhtml_boundary =
      WebString::FromUTF8(params->mhtml_boundary_marker);
  DCHECK(!mhtml_boundary.IsEmpty());

  std::vector<WebThreadSafeData> mhtml_contents;
  std::unordered_set<std::string> serialized_resources_uri_digests;
  MHTMLPartsGenerationDelegate delegate(*params,
                                        &serialized_resources_uri_digests);

  mojom::MhtmlSaveStatus save_status = mojom::MhtmlSaveStatus::kSuccess;
  bool has_some_data = false;

  if (IsMainFrame()) {
    TRACE_EVENT0("page-serialization",
                 "RenderFrameImpl::SerializeAsMHTML header");
    mhtml_contents.emplace_back(WebFrameSerializer::GenerateMHTMLHeader(
        mhtml_boundary, GetWebFrame(), &delegate));
    has_some_data = true;
  }

  if (save_status == mojom::MhtmlSaveStatus::kSuccess) {
    TRACE_EVENT0("page-serialization",
                 "RenderFrameImpl::SerializeAsMHTML parts serialization");
    mhtml_contents.emplace_back(WebFrameSerializer::GenerateMHTMLParts(
        mhtml_boundary, GetWebFrame(), &delegate));
    has_some_data |= !mhtml_contents.back().IsEmpty();
  }


  base::TimeDelta main_thread_use_time = base::TimeTicks::Now() - start_time;
  UMA_HISTOGRAM_TIMES(
      "PageSerialization.MhtmlGeneration.RendererMainThreadTime.SingleFrame",
      main_thread_use_time);

  MHTMLHandleWriterDelegate handle_delegate(
      *params,
      base::BindOnce(&RenderFrameImpl::OnWriteMHTMLComplete,
                     weak_factory_.GetWeakPtr(), std::move(callback),
                     std::move(serialized_resources_uri_digests),
                     main_thread_use_time),
      GetTaskRunner(blink::TaskType::kInternalDefault));

  if (save_status == mojom::MhtmlSaveStatus::kSuccess && has_some_data) {
    handle_delegate.WriteContents(mhtml_contents);
  } else {
    handle_delegate.Finish(save_status);
  }
}
