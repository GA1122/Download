void RequestSender::HandleSendError(int error, int retry_after_sec) {
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(std::move(request_sender_callback_), error,
                                std::string(), retry_after_sec));
}
