#include "parsing/parsing.h"
#include <stdio.h>

void free_config(t_config *cfg)
{
    int i;

    i = 0;
    while (i < TEX_MAX)
    {
        free(cfg->tex[i]);
        cfg->tex[i] = NULL;
        i++;
    }
    i = 0;
    while (i < TEX_MAX)
    {
        cfg->has_tex[i] = 0;
        i++;
    }
    cfg->floor_set = 0;
    cfg->ceil_set = 0;
    cfg->header_done = 0;
}
void free_map(t_map *m)
{
    int y = 0;
    if (!m || !m->rows)
        return;
    while (y < m->height)
    {
        free(m->rows[y]);
        y++;
    }
    free(m->rows);
    memset(m, 0, sizeof(*m));
}

static void print_cfg(t_config *cfg)
{
    int i = 0;
    const char *name[TEX_MAX] = {"NO", "SO", "WE", "EA"};

    while (i < TEX_MAX)
    {
        printf("%s = %s\n", name[i], cfg->tex[i] ? cfg->tex[i] : "(null)");
        i++;
    }
    printf("F = %d,%d,%d\n", cfg->floor_rgb.r, cfg->floor_rgb.g, cfg->floor_rgb.b);
    printf("C = %d,%d,%d\n", cfg->ceil_rgb.r, cfg->ceil_rgb.g, cfg->ceil_rgb.b);
}

static void print_map(t_map *m)
{
    int y = 0;

    printf("Map: %dx%d | Player (%d,%d) dir %c\n",
           m->width, m->height, m->player_x, m->player_y, m->player_dir);
    while (y < m->height)
    {
        fwrite(m->rows[y], 1, (size_t)m->width, stdout);
        fputc('\n', stdout);
        y++;
    }
}

int main(int argc, char **argv)
{
    t_config cfg;
    t_map map;

    if (argc != 2)
        return (fprintf(stderr, "Usage: %s file.cub\n", argv[0]), 1);
    if (parse_cub_file(argv[1], &cfg, &map))
        return 1;
    print_cfg(&cfg);
    print_map(&map);
    free_map(&map);
    free_config(&cfg);
    return 0;
}
