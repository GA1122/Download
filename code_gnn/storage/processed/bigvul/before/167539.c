void WebMediaPlayerImpl::SetSinkId(
    const blink::WebString& sink_id,
    blink::WebSetSinkIdCallbacks* web_callback) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());
  DVLOG(1) << __func__;

  media::OutputDeviceStatusCB callback =
      media::ConvertToOutputDeviceStatusCB(web_callback);
  media_task_runner_->PostTask(
      FROM_HERE, base::Bind(&SetSinkIdOnMediaThread, audio_source_provider_,
                            sink_id.Utf8(), callback));
}
