void SparseHistogram::WriteHTMLGraph(std::string* output) const {
  output->append("<PRE>");
  WriteAsciiImpl(true, "<br>", output);
  output->append("</PRE>");
}
