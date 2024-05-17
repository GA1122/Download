  ColorChooser(content::ColorChooser* chooser,
               blink::mojom::ColorChooserRequest request,
               blink::mojom::ColorChooserClientPtr client)
      : chooser_(chooser),
        binding_(this, std::move(request)),
        client_(std::move(client)) {
    binding_.set_connection_error_handler(
        base::BindOnce([](content::ColorChooser* chooser) { chooser->End(); },
                       base::Unretained(chooser)));
  }
