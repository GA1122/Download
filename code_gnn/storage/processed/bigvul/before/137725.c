  static void Create(chrome::mojom::ZipFileCreatorRequest request) {
    mojo::MakeStrongBinding(base::MakeUnique<ZipFileCreatorImpl>(),
                            std::move(request));
  }
