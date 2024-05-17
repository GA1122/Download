void ImageResource::AppendData(const char* data, size_t length) {
  v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(length);
  if (multipart_parser_) {
    multipart_parser_->AppendData(data, length);
  } else {
    Resource::AppendData(data, length);

    if (GetContent()->ShouldUpdateImageImmediately()) {
      UpdateImage(Data(), ImageResourceContent::kUpdateImage, false);
      return;
    }

    if (!flush_timer_.IsActive()) {
      double now = WTF::CurrentTimeTicksInSeconds();
      if (!last_flush_time_)
        last_flush_time_ = now;

      DCHECK_LE(last_flush_time_, now);
      double flush_delay = last_flush_time_ - now + kFlushDelaySeconds;
      if (flush_delay < 0.)
        flush_delay = 0.;
      flush_timer_.StartOneShot(flush_delay, FROM_HERE);
    }
  }
}
