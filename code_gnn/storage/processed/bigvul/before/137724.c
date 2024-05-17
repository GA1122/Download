  static void Create(chrome::mojom::FilePatcherRequest request) {
    mojo::MakeStrongBinding(base::MakeUnique<FilePatcherImpl>(),
                            std::move(request));
  }
