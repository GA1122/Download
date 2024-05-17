void BlinkTestRunner::CaptureDumpPixels(const SkBitmap& snapshot) {
  DCHECK_NE(0, snapshot.info().fWidth);
  DCHECK_NE(0, snapshot.info().fHeight);

  SkAutoLockPixels snapshot_lock(snapshot);
  MSAN_UNPOISON(snapshot.getPixels(), snapshot.getSize());
  base::MD5Digest digest;
  base::MD5Sum(snapshot.getPixels(), snapshot.getSize(), &digest);
  std::string actual_pixel_hash = base::MD5DigestToBase16(digest);

  if (actual_pixel_hash == test_config_.expected_pixel_hash) {
    SkBitmap empty_image;
    Send(new ShellViewHostMsg_ImageDump(
        routing_id(), actual_pixel_hash, empty_image));
  } else {
    Send(new ShellViewHostMsg_ImageDump(
        routing_id(), actual_pixel_hash, snapshot));
  }

  CaptureDumpComplete();
}
