  static uint32_t GetURLLoaderOptions(bool is_main_frame) {
    uint32_t options = network::mojom::kURLLoadOptionNone;

    options |= network::mojom::kURLLoadOptionSniffMimeType;

    if (is_main_frame) {
      options |= network::mojom::kURLLoadOptionSendSSLInfoWithResponse;
      options |= network::mojom::kURLLoadOptionSendSSLInfoForCertificateError;
    }

    if (!base::FeatureList::IsEnabled(network::features::kNetworkService)) {
      options |= network::mojom::kURLLoadOptionPauseOnResponseStarted;
    }

    return options;
  }
