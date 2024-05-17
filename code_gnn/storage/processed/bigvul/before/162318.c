void MojoJpegDecodeAccelerator::InitializeAsync(Client* client,
                                                InitCB init_cb) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());

  jpeg_decoder_.Bind(std::move(jpeg_decoder_info_));

  jpeg_decoder_.set_connection_error_handler(
      base::Bind(&MojoJpegDecodeAccelerator::OnLostConnectionToJpegDecoder,
                 base::Unretained(this)));
  jpeg_decoder_->Initialize(
      base::Bind(&MojoJpegDecodeAccelerator::OnInitializeDone,
                 base::Unretained(this), std::move(init_cb), client));
}
