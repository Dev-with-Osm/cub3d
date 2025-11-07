#include "cube.h"

/* * Converts a floating-point angle (in radians) into a normalized 
 * 0-359 degree index for the look-up tables.
 */
int angle_to_index(float angle)
{
    // Normalize angle to be between 0 and 2*PI
    angle = fmod(angle, 2 * PI);
    if (angle < 0) 
        angle += 2 * PI;
    
    // Convert to 0-360 degrees and cast to index
    return (int)(angle * 180 / PI) % TR_TABLE_SIZE;
}

/* * Allocates and populates the sine and cosine look-up tables.
 */
void setup_trig_tables(t_game *game)
{
    int i;
    
    // Allocate memory using the garbage collector
    game->cos_table = w_malloc(sizeof(float) * TR_TABLE_SIZE);
    game->sin_table = w_malloc(sizeof(float) * TR_TABLE_SIZE);

    i = 0;
    while (i < TR_TABLE_SIZE)
    {
        float rad = DEG_TO_RAD(i); // Convert degree index to radians
        game->cos_table[i] = cos(rad);
        game->sin_table[i] = sin(rad);
        i++;
    }
}