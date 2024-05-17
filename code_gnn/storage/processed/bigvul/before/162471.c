  ImageResourceFactory(const FetchParameters& fetch_params)
      : NonTextResourceFactory(Resource::kImage),
        fetch_params_(&fetch_params) {}
