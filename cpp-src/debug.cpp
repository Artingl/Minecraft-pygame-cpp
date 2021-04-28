#include <cstdio>
#include <cstdlib>

void _gl_engine_handle_error(const char* prefix, const char* msg ) {
    printf("[ERROR] [ENGINE] [%s]: %s\n", prefix, msg);
    std::exit(1);
}

void _gl_engine_info(const char* prefix, const char* msg ) {
    printf("[INFO] [ENGINE] [%s]: %s\n", prefix, msg);
}


void _gl_engine_warn(const char* prefix, const char* msg ) {
    printf("[WARNING] [ENGINE] [%s]: %s\n", prefix, msg);
}

