void SetWithinSameDocument(
    const GURL& url,
    FrameHostMsg_DidCommitProvisionalLoad_Params* params) {
  params->was_within_same_document = true;
  params->url = url;
  params->origin = url::Origin(url);
}
