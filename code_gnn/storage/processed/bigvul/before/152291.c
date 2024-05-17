void RenderFrameImpl::DidSerializeDataForFrame(
    const WebVector<char>& data,
    WebFrameSerializerClient::FrameSerializationStatus status) {
  bool end_of_data =
      status == WebFrameSerializerClient::kCurrentFrameIsFinished;
  Send(new FrameHostMsg_SerializedHtmlWithLocalLinksResponse(
      routing_id_, std::string(data.Data(), data.size()), end_of_data));
}
