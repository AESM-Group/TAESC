#pragma once
/**
 * @file taesc_options.h
 * @brief TAESC runtime options facade.
 */

#define TAESC_MAX_STRLEN 200

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    /** Internal instance ID used by the manager. Do not modify manually. */
    const int instance_id_;
} TaescOptions;

TaescOptions* taesc_create_options(void);
void taesc_destroy_options(TaescOptions *opts);
void taesc_set_option(TaescOptions *opts, const char *key, const char *value);

#ifdef __cplusplus
}
#endif
