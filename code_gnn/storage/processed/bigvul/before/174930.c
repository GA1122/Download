status_t Camera2Client::updateProcessorStream(sp<ProcessorT> processor,
                                              camera2::Parameters params) {
 return updateProcessorStream<ProcessorT, &ProcessorT::updateStream>(
            processor, params);
}
