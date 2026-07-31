#pragma once
/**
 * @file taesc_global.h
 * @brief TAESC global environment facade for LibRPA.
 */

#include "taesc_enums.h"

#ifdef __cplusplus
extern "C" {
#endif

const char* taesc_get_build_info(void);
int taesc_get_major_version(void);
int taesc_get_minor_version(void);
int taesc_get_patch_version(void);
#ifdef __cplusplus
void taesc_init_global(TaescSwitch switch_redirect_stdout = TAESC_SWITCH_OFF,
                       const char *redirect_path = "stdout",
                       TaescSwitch switch_process_output = TAESC_SWITCH_ON);
#else
void taesc_init_global(TaescSwitch switch_redirect_stdout,
                       const char *redirect_path,
                       TaescSwitch switch_process_output);
#endif
void taesc_set_output_level(TaescVerbose output_level);
TaescVerbose taesc_get_output_level(void);
void taesc_finalize_global(void);
void taesc_test(void);
void taesc_print_profile(void);

#ifdef __cplusplus
}
#endif
