  void WriteContents(std::vector<WebThreadSafeData> mhtml_contents) {
    base::PostTaskWithTraits(
        FROM_HERE, {base::MayBlock()},
        base::BindOnce(&MHTMLHandleWriter::WriteContents,
                       base::Unretained(handle_), std::move(mhtml_contents)));
  }
