MojoJpegDecodeAccelerator::MojoJpegDecodeAccelerator(
    scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
    mojom::JpegDecodeAcceleratorPtrInfo jpeg_decoder)
    : io_task_runner_(std::move(io_task_runner)),
      jpeg_decoder_info_(std::move(jpeg_decoder)) {}
