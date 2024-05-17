void HeapObjectHeader::ZapMagic() {
  CheckHeader();
  magic_ = kZappedMagic;
}
