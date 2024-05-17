void ForwardError(syncer::OnceModelErrorHandler error_handler,
                  const base::Optional<syncer::ModelError>& error) {
  if (error) {
    std::move(error_handler).Run(*error);
  }
}
