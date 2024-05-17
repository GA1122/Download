  void TestDidNavigate(int page_id, const GURL& url) {
    ViewHostMsg_FrameNavigate_Params params;
    InitNavigateParams(&params, page_id, url, content::PAGE_TRANSITION_TYPED);
    DidNavigate(GetRenderViewHostForTesting(), params);
  }
