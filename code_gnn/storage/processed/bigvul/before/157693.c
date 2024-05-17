void SetOriginalURL(const GURL& url,
                    FrameHostMsg_DidCommitProvisionalLoad_Params* params) {
  params->original_request_url = url;
}
