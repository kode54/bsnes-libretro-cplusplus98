#ifndef LIBRETRO_H__
#define LIBRETRO_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

typedef struct sfm_file sfm_file;

sfm_file * sfm_load_state(uint8_t const* state, size_t size);
size_t sfm_get_state_size(sfm_file const* state);
void sfm_save_state(sfm_file const* state, uint8_t * out, size_t out_size);
void sfm_free_state(sfm_file * state);

typedef struct sfm_player sfm_player;

sfm_player * sfm_player_create();
void sfm_player_free(sfm_player * player);
void sfm_player_load_state(sfm_player * player, sfm_file const* state);
void sfm_player_save_state(sfm_player const* player, sfm_file * state);
void sfm_player_render(sfm_player * player, uint16_t * sample_buffer, size_t count);

#ifdef __cplusplus
}
#endif

#endif
