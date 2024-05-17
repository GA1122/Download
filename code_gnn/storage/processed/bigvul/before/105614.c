 bool ForbidsMessageBody(const std::string& request_method,
                         const HttpResponse& response) {
   return request_method == "HEAD" ||
         response.status() == HttpResponse::kNoContent ||
         response.status() == HttpResponse::kNotModified ||
         (response.status() >= 100 && response.status() < 200);
}
