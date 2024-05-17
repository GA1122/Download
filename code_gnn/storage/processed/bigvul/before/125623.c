void RenderViewHostImpl::InsertCSS(const string16& frame_xpath,
                                   const std::string& css) {
  Send(new ViewMsg_CSSInsertRequest(GetRoutingID(), frame_xpath, css));
}
