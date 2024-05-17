  static void Create(chrome::mojom::SafeArchiveAnalyzerRequest request) {
    mojo::MakeStrongBinding(base::MakeUnique<SafeArchiveAnalyzerImpl>(),
                            std::move(request));
  }
