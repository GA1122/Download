FakeCentral::FakeCentral(mojom::CentralState state,
                         mojom::FakeCentralRequest request)
    : state_(state), binding_(this, std::move(request)) {}
