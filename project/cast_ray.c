#include "cube.h"

void delta_ins(t_dda *dda)
{
     // Calculates the distance needed to travel along the ray to move 1 unit in X or Y
    if(dda->ray_dir_x == 0)
        dda->delta_dist_x =  1e30;
    else
        dda->delta_dist_x = fabs(1 / dda->ray_dir_x);
    if(dda->ray_dir_y == 0)
        dda->delta_dist_y =  1e30;
    else
        dda->delta_dist_y =  fabs(1 / dda->ray_dir_y);
}

void ft_init_dda(t_game *game, float ray_angle, t_dda *dda)
{
    dda->map_x = (int)game->player->pp_x;
    dda->map_y = (int)game->player->pp_y;
    dda->ray_dir_x = cos(ray_angle);
    dda->ray_dir_y = sin(ray_angle);
    delta_ins(dda);
    // Calculate step and initial sideDist (distance to the first grid line)
    if (dda->ray_dir_x < 0) {
        dda->step_x = -1;
        dda->side_dist_x = (game->player->pp_x - dda->map_x) * dda->delta_dist_x;
    } else {
        dda->step_x = 1;
        dda->side_dist_x = (dda->map_x + 1.0 - game->player->pp_x) * dda->delta_dist_x;
    }
    if (dda->ray_dir_y < 0) {
        dda->step_y = -1;
        dda->side_dist_y = (game->player->pp_y - dda->map_y) * dda->delta_dist_y;
    } else {
        dda->step_y = 1;
        dda->side_dist_y = (dda->map_y + 1.0 - game->player->pp_y) * dda->delta_dist_y;
    }
}

float ft_dda_traversal(t_game *game, t_dda *dda)
{
    int hit = 0;

    // Perform DDA: Jump from square to square until a wall is found
    while (hit == 0) {
        if (dda->side_dist_x < dda->side_dist_y) {
            dda->side_dist_x += dda->delta_dist_x;
            dda->map_x += dda->step_x;
            dda->side = 0; // Hit X-side (Vertical grid line)
        } else {
            dda->side_dist_y += dda->delta_dist_y;
            dda->map_y += dda->step_y;
            dda->side = 1; // Hit Y-side (Horizontal grid line)
        }

        if (dda->map_x < 0 || dda->map_x >= game->map_W || 
            dda->map_y < 0 || dda->map_y >= game->map_H)
            return (game->max_distance); // Hit boundary
        if (game->map[dda->map_y][dda->map_x] == '1')
            hit = 1;
    }

    // Calculate perpendicular distance to avoid fisheye effect
    if (dda->side == 0)
        return (dda->side_dist_x - dda->delta_dist_x);
    else
        return (dda->side_dist_y - dda->delta_dist_y);
}


void ft_calculate_hit_data(t_game *game, t_dda *dda, float perp_dist, t_wall *wall_data)
{
    float wall_x_raw; 

    wall_data->wall_distance = perp_dist;
    if (dda->side == 0) // X-side hit (Vertical grid line)
    {
        // Calculate exact hit point on the wall in world coordinates
        wall_x_raw = game->player->pp_y + perp_dist * dda->ray_dir_y;
        // Set wall face (fixed logic)
        if(dda->ray_dir_x > 0)
            wall_data->wall_face = WEST;
        else
            wall_data->wall_face = EAST;
    }
    else // Y-side hit (Horizontal grid line)
    {
        // Calculate exact hit point on the wall in world coordinates
        wall_x_raw = game->player->pp_x + perp_dist * dda->ray_dir_x;
        // Set wall face (fixed logic)
        if(dda->ray_dir_y > 0)
            wall_data->wall_face = NORTH;
        else 
            wall_data->wall_face = SOUTH;
    }
    // Get the fractional part (0.0 to 1.0) of the coordinate
    wall_x_raw -= floor(wall_x_raw); 
    wall_data->wall_x = wall_x_raw;
}

float cast_ray(t_game *game, float ray_angle, t_wall *wall_data)
{
    t_dda   dda;
    float   perp_wall_dist;

    // 1. Initialize Ray Variables
    ft_init_dda(game, ray_angle, &dda);

    // 2. Perform DDA Traversal
    perp_wall_dist = ft_dda_traversal(game, &dda);
    
    if (perp_wall_dist == game->max_distance)
        return (game->max_distance);

    // 3. Calculate Wall Hit Data for Texturing
    ft_calculate_hit_data(game, &dda, perp_wall_dist, wall_data);

    return (perp_wall_dist);
}