    void CallCallback(UsageAndQuotaCallback callback) {
      std::move(callback).Run(blink::mojom::QuotaStatusCode::kOk, 0,
                              kMockQuota);
    }
