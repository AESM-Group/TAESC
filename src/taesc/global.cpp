#include "taesc/taesc_global.h"

#include "librpa_global.h"


const char* taesc_get_build_info(void)
{
    return librpa_get_build_info();
}

int taesc_get_major_version(void)
{
    return librpa_get_major_version();
}

int taesc_get_minor_version(void)
{
    return librpa_get_minor_version();
}

int taesc_get_patch_version(void)
{
    return librpa_get_patch_version();
}

void taesc_init_global()
{
    librpa_init_global(false, "stdout", true);
}

void taesc_finalize_global(void)
{
    librpa_finalize_global();
}

void taesc_print_profile(void)
{
    librpa_print_profile();
}
