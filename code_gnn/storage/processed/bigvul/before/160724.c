void RenderFrameImpl::SendFindReply(int request_id,
                                    int match_count,
                                    int ordinal,
                                    const WebRect& selection_rect,
                                    bool final_status_update) {
  DCHECK_GE(ordinal, -1);
  WebRect converted_rect = selection_rect;

  GetRenderWidget()->ConvertViewportToWindow(&converted_rect);

  Send(new FrameHostMsg_Find_Reply(routing_id_,
                                   request_id,
                                   match_count,
                                   converted_rect,
                                   ordinal,
                                   final_status_update));
}
