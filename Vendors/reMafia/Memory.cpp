/*
    NOTE(DavoSK): we redirect our new operator allocation into Game.exe operators
    so we ensure with this correct memory interop between our code and engine's one
*/

/* ---------------------------*/
void* operator new(size_t size) {
    return ((void*(__cdecl*)(size_t))(0x6106FC))(size);
}

void operator delete(void* p) {
    ((void(__cdecl*)(void*))(0x60FCF6))(p);
}

void* game_malloc(size_t size) {
    return ((void*(__cdecl*)(size_t))(0x612D80))(size);
}

void game_free(void* ptr) {
    ((void(__cdecl*)(void*))(0x6124F3))(ptr);
}
/* ---------------------------*/