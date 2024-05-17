void ImageProcessorClient::CreateImageProcessorTask(
    const ImageProcessor::PortConfig& input_config,
    const ImageProcessor::PortConfig& output_config,
    size_t num_buffers,
    base::WaitableEvent* done) {
  DCHECK_CALLED_ON_VALID_THREAD(image_processor_client_thread_checker_);
  image_processor_ = ImageProcessorFactory::Create(
      input_config, output_config, {ImageProcessor::OutputMode::IMPORT},
      num_buffers,
      base::BindRepeating(&ImageProcessorClient::NotifyError,
                          base::Unretained(this)));
  done->Signal();
}
