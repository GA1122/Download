void Shutdown(struct mg_connection* connection,
              const struct mg_request_info* request_info,
              void* user_data) {
  base::WaitableEvent* shutdown_event =
      reinterpret_cast<base::WaitableEvent*>(user_data);
  mg_printf(connection, "HTTP/1.1 200 OK\r\n\r\n");
   shutdown_event->Signal();
 }
