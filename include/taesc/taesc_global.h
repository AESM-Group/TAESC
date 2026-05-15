#pragma once
/**
 * @file taesc_global.h
 * @brief TAESC global environment facade for LibRPA.
 */

#ifdef __cplusplus
extern "C" {
#endif

const char* taesc_get_build_info(void);
int taesc_get_major_version(void);
int taesc_get_minor_version(void);
int taesc_get_patch_version(void);
void taesc_init_global(void);
void taesc_finalize_global(void);

#ifdef __cplusplus
}
#endif
