  static void Bind(int process_id, int routing_id,
                   media::mojom::RemoterFactoryRequest request) {
    mojo::MakeStrongBinding(
        base::MakeUnique<RemoterFactoryImpl>(process_id, routing_id),
        std::move(request));
  }
