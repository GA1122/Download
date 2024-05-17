void RecordDeliveryStatus(content::mojom::PushDeliveryStatus status) {
  UMA_HISTOGRAM_ENUMERATION(
      "PushMessaging.DeliveryStatus", status,
      static_cast<int>(content::mojom::PushDeliveryStatus::LAST) + 1);
}
