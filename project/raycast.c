#include "cube.h"

static void init_ray(t_game *game, float angle, t_dda *dda)
{
    dda->map_x = (int)game->map->pp_x;
    dda->map_y = (int)game->map->pp_y;
    
    dda->ray_dir_x = cos(angle);
    dda->ray_dir_y = sin(angle);
    
    // Calculate delta distances
    dda->delta_dist_x = (dda->ray_dir_x == 0) ? 1e30 : fabs(1.0f / dda->ray_dir_x);
    dda->delta_dist_y = (dda->ray_dir_y == 0) ? 1e30 : fabs(1.0f / dda->ray_dir_y);
    
    // Calculate step and initial side distances
    if (dda->ray_dir_x < 0)
    {
        dda->step_x = -1;
        dda->side_dist_x = (game->map->pp_x - dda->map_x) * dda->delta_dist_x;
    }
    else
    {
        dda->step_x = 1;
        dda->side_dist_x = (dda->map_x + 1.0f - game->map->pp_x) * dda->delta_dist_x;
    }
    
    if (dda->ray_dir_y < 0)
    {
        dda->step_y = -1;
        dda->side_dist_y = (game->map->pp_y - dda->map_y) * dda->delta_dist_y;
    }
    else
    {
        dda->step_y = 1;
        dda->side_dist_y = (dda->map_y + 1.0f - game->map->pp_y) * dda->delta_dist_y;
    }
}

static float perform_dda(t_game *game, t_dda *dda)
{
    int hit = 0;
    
    while (!hit)
    {
        // Jump to next grid square
        if (dda->side_dist_x < dda->side_dist_y)
        {
            dda->side_dist_x += dda->delta_dist_x;
            dda->map_x += dda->step_x;
            dda->side = 0;
        }
        else
        {
            dda->side_dist_y += dda->delta_dist_y;
            dda->map_y += dda->step_y;
            dda->side = 1;
        }
        
        // Check boundaries
        if (dda->map_x < 0 || dda->map_x >= game->map->map_W ||
            dda->map_y < 0 || dda->map_y >= game->map->map_H)
            return (game->max_distance);
        
        // Check if we hit a wall
        if (game->map->map[dda->map_y][dda->map_x] == '1')
            hit = 1;
    }
    
    // Calculate perpendicular distance
    if (dda->side == 0)
        return (dda->side_dist_x - dda->delta_dist_x);
    return (dda->side_dist_y - dda->delta_dist_y);
}

static void calculate_wall_data(t_game *game, t_dda *dda, float distance)
{
    float wall_hit;
    
    // Calculate where on the wall we hit
    if (dda->side == 0)
    {
        wall_hit = game->map->pp_y + distance * dda->ray_dir_y;
        game->wall->wall_face = (dda->step_x > 0) ? EAST : WEST;
    }
    else
    {
        wall_hit = game->map->pp_x + distance * dda->ray_dir_x;
        game->wall->wall_face = (dda->step_y > 0) ? SOUTH : NORTH;
    }
    
    // Get fractional part for texture coordinate
    game->wall->wall_x = wall_hit - floor(wall_hit);
    
    // Ensure valid range
    if (game->wall->wall_x < 0.0f)
        game->wall->wall_x += 1.0f;
    if (game->wall->wall_x >= 1.0f)
        game->wall->wall_x = 0.99999f;
    
    game->wall->wall_distance = distance;
}

float cast_ray(t_game *game, float angle)
{
    t_dda dda;
    float distance;
    
    // Initialize ray
    init_ray(game, angle, &dda);
    
    // Perform DDA
    distance = perform_dda(game, &dda);
    
    // Calculate wall hit data
    if (distance < game->max_distance)
        calculate_wall_data(game, &dda, distance);
    
    return (distance);
}