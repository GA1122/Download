void ImageResource::AllClientsAndObserversRemoved() {
  CHECK(is_during_finish_as_error_ || !GetContent()->HasImage() ||
        !ErrorOccurred());
  if (!ThreadHeap::WillObjectBeLazilySwept(this)) {
    Platform::Current()->CurrentThread()->GetWebTaskRunner()->PostTask(
        FROM_HERE, WTF::Bind(&ImageResourceContent::DoResetAnimation,
                             WrapWeakPersistent(GetContent())));
  } else {
    GetContent()->DoResetAnimation();
  }
  if (multipart_parser_)
    multipart_parser_->Cancel();
  Resource::AllClientsAndObserversRemoved();
}
