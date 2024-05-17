  void Finish(mojom::MhtmlSaveStatus save_status) {
    base::PostTaskWithTraits(
        FROM_HERE, {base::MayBlock()},
        base::BindOnce(&MHTMLHandleWriter::Finish, base::Unretained(handle_),
                       save_status));
  }
