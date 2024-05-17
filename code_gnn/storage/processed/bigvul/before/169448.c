 FileStream::~FileStream() {
   context_.release()->Orphan();
}
