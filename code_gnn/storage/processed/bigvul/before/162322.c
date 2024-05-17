void MojoJpegDecodeAccelerator::OnLostConnectionToJpegDecoder() {
  DCHECK(io_task_runner_->BelongsToCurrentThread());
  OnDecodeAck(kInvalidBitstreamBufferId,
              ::media::JpegDecodeAccelerator::Error::PLATFORM_FAILURE);
}
