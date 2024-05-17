void VaapiVideoDecodeAccelerator::TryOutputSurface() {
  DCHECK_EQ(message_loop_, base::MessageLoop::current());

  if (!client_)
    return;

  if (pending_output_cbs_.empty() || output_buffers_.empty())
    return;

  OutputCB output_cb = pending_output_cbs_.front();
  pending_output_cbs_.pop();

  VaapiPicture* picture = PictureById(output_buffers_.front());
  DCHECK(picture);
  output_buffers_.pop();

  output_cb.Run(picture);

  if (finish_flush_pending_ && pending_output_cbs_.empty())
    FinishFlush();
}
