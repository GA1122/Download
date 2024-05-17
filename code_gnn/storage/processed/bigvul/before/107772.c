void FrameLoader::SetReferrerForFrameRequest(FrameLoadRequest& frame_request) {
  ResourceRequest& request = frame_request.GetResourceRequest();
  Document* origin_document = frame_request.OriginDocument();

  if (!origin_document)
    return;
  if (frame_request.GetShouldSendReferrer() == kNeverSendReferrer)
    return;

  String referrer_to_use = request.ReferrerString();
  network::mojom::ReferrerPolicy referrer_policy_to_use =
      request.GetReferrerPolicy();

  if (referrer_to_use == Referrer::ClientReferrerString())
    referrer_to_use = origin_document->OutgoingReferrer();

  if (referrer_policy_to_use == network::mojom::ReferrerPolicy::kDefault)
    referrer_policy_to_use = origin_document->GetReferrerPolicy();

  Referrer referrer = SecurityPolicy::GenerateReferrer(
      referrer_policy_to_use, request.Url(), referrer_to_use);

  request.SetHTTPReferrer(referrer);
  request.SetHTTPOriginToMatchReferrerIfNeeded();
}
