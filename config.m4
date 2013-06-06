PHP_ARG_ENABLE(cpuid, whether to enable cpuid support,
[ --enable-cpuid   Enable cpuid support])

if test "$PHP_CPUID" = "yes"; then
  PHP_ADD_LIBPATH(/usr/lib)
  AC_DEFINE(HAVE_CPUID, 1, [Whether you have cpuid])
  PHP_NEW_EXTENSION(cpuid, cpuid.c LibCpuId.c, $ext_shared)
fi
