UmaIDBException ExceptionCodeToUmaEnum(uint16_t code) {
  switch (code) {
    case blink::kWebIDBDatabaseExceptionUnknownError:
      return UmaIDBExceptionUnknownError;
    case blink::kWebIDBDatabaseExceptionConstraintError:
      return UmaIDBExceptionConstraintError;
    case blink::kWebIDBDatabaseExceptionDataError:
      return UmaIDBExceptionDataError;
    case blink::kWebIDBDatabaseExceptionVersionError:
      return UmaIDBExceptionVersionError;
    case blink::kWebIDBDatabaseExceptionAbortError:
      return UmaIDBExceptionAbortError;
    case blink::kWebIDBDatabaseExceptionQuotaError:
      return UmaIDBExceptionQuotaError;
    case blink::kWebIDBDatabaseExceptionTimeoutError:
      return UmaIDBExceptionTimeoutError;
    default:
      NOTREACHED();
  }
  return UmaIDBExceptionUnknownError;
}
