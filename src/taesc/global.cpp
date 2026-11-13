#include "taesc/taesc_global.h"

#include "librpa_global.h"

#include "../version.h"


const char* taesc_get_build_info(void)
{
    return librpa_get_build_info();
}

int taesc_get_major_version(void)
{
    return TAESC_MAJOR_VERSION;
}

int taesc_get_minor_version(void)
{
    return TAESC_MINOR_VERSION;
}

int taesc_get_patch_version(void)
{
    return TAESC_PATCH_VERSION;
}

void taesc_init_global(TaescSwitch switch_redirect_stdout,
                       const char *redirect_path,
                       TaescSwitch switch_process_output)
{
    librpa_init_global(switch_redirect_stdout, redirect_path,
                       switch_process_output);
}

void taesc_set_output_level(TaescVerbose output_level)
{
    librpa_set_output_level(output_level);
}

TaescVerbose taesc_get_output_level(void)
{
    return librpa_get_output_level();
}

void taesc_finalize_global(void)
{
    librpa_finalize_global();
}

void taesc_test(void)
{
    librpa_test();
}

void taesc_print_profile(void)
{
    librpa_print_profile();
}
