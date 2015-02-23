add_library(a OBJECT a.c)
target_sources(a PRIVATE requires.c)
target_sources(a INTERFACE b.c)
