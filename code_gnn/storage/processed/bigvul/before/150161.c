void ImageProcessorClient::Process(const Image& input_image,
                                   const Image& output_image) {
  DCHECK_CALLED_ON_VALID_THREAD(test_main_thread_checker_);
  auto input_frame = CreateInputFrame(input_image);
  ASSERT_TRUE(input_frame);
  auto output_frame = CreateOutputFrame(output_image);
  ASSERT_TRUE(output_frame);
  image_processor_client_thread_.task_runner()->PostTask(
      FROM_HERE,
      base::BindOnce(&ImageProcessorClient::ProcessTask, base::Unretained(this),
                     std::move(input_frame), std::move(output_frame)));
}
