extern int a(void);
int b(void) { return a(); }
#ifndef REQUIRED
#error "REQUIRED needs to be defined"
#endif
