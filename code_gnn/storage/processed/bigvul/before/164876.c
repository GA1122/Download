DownloadUrlParameters::DownloadUrlParameters(
    const GURL& url,
    const net::NetworkTrafficAnnotationTag& traffic_annotation)
    : DownloadUrlParameters(url,
                            -1,
                            -1,
                            -1,
                            traffic_annotation) {}
