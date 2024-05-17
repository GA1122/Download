v8::Handle<v8::Value> V8Proxy::throwError(ErrorType type, const char* message, v8::Isolate* isolate)
{
    switch (type) {
    case RangeError:
        return v8::ThrowException(v8::Exception::RangeError(v8String(message, isolate)));
    case ReferenceError:
        return v8::ThrowException(v8::Exception::ReferenceError(v8String(message, isolate)));
    case SyntaxError:
        return v8::ThrowException(v8::Exception::SyntaxError(v8String(message, isolate)));
    case TypeError:
        return v8::ThrowException(v8::Exception::TypeError(v8String(message, isolate)));
    case GeneralError:
        return v8::ThrowException(v8::Exception::Error(v8String(message, isolate)));
    default:
        ASSERT_NOT_REACHED();
        return notHandledByInterceptor();
    }
}
