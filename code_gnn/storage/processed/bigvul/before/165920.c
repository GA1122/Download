void RenderFrameImpl::OnPostMessageEvent(FrameMsg_PostMessage_Params params) {

  params.message->data.EnsureDataIsOwned();

  frame_->GetTaskRunner(blink::TaskType::kPostedMessage)
      ->PostTask(FROM_HERE,
                 base::BindOnce(&RenderFrameImpl::PostMessageEvent,
                                weak_factory_.GetWeakPtr(), std::move(params)));
}
