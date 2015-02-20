extern int b(void);
int main()
{
    return b();
}
#ifndef REQUIRED
#error "REQUIRED needs to be defined"
#endif
