void ImageResource::FinishAsError(const ResourceError& error,
                                  base::SingleThreadTaskRunner* task_runner) {
  if (multipart_parser_)
    multipart_parser_->Cancel();
  SetEncodedSize(0);
  is_during_finish_as_error_ = true;
  Resource::FinishAsError(error, task_runner);
  is_during_finish_as_error_ = false;
  UpdateImage(nullptr, ImageResourceContent::kClearImageAndNotifyObservers,
              true);
}
