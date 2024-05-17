CrosLibrary::CrosLibrary() : library_loader_(NULL),
                             own_library_loader_(false),
                             use_stub_impl_(false),
                             loaded_(false),
                             load_error_(false),
                             test_api_(NULL) {
}
