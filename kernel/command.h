#ifndef _COMMAND_H
#define _COMMAND_H

typedef void (*command_func_t)(char *args);

typedef struct {
    const char *name;
    command_func_t func;
    const char *help;
} command_t;

extern command_t disk_part_cmds[];
extern command_t disk_cmds[];
extern command_t fs_cmds[];
extern command_t commands[];

#endif
