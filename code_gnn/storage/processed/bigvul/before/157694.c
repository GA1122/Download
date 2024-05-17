void SetRedirects(const std::vector<GURL>& redirects,
                  FrameHostMsg_DidCommitProvisionalLoad_Params* params) {
  params->redirects = redirects;
}
