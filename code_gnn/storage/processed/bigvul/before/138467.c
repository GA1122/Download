void Microtask::enqueueMicrotask(PassOwnPtr<Closure> callback)
{
    enqueueMicrotask(adoptPtr(new Task(callback)));
}
