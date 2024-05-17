  void CompleteSavingAsAlreadyExists() {
    DCHECK(mock_saving_);
    mock_saving_ = false;
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::BindOnce(std::move(save_page_callback_),
                                  SavePageResult::ALREADY_EXISTS, 123456));
  }
