#ifndef POP_H
#define POP_H

/*-----------------------------------------------------------------------------
 *        Definition
 *----------------------------------------------------------------------------*/

/*-----MENU_POP---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
extern void PopDraw(void);
extern void PopMsg(u32 uMsg);
extern i8   iCmdOpen;

/*----------------------------------------------------------------------------*/
/*------------------√¸¡Ó“ª¿¿--------------------------------------------------*/
#define POP_CMD_LIST_CTRL   0
    #define POP_CTRL_END      0x000
    #define POP_CTRL_NOP      0x001
    #define POP_CTRL_LINE     0x002
#define POP_CMD_LIST_MOV    1
    #define POP_MOV_JNT       0x103
    #define POP_MOV_LINE      0x104
    #define POP_MOV_CIRC      0x105
#define POP_CMD_LIST_IO     2
    #define POP_IO_SET        0x206
    #define POP_IO_WAIT       0x207
    #define POP_IO_DLY        0x208
#define POP_CMD_LIST_PROC   3
    #define POP_PROC_LABEL    0x309
    #define POP_PROC_GOTO     0x30A
    #define POP_PROC_NOTE     0x30B
#define POP_CMD_LIST_OPRT   4
    #define POP_OPRT_DECR     0x40C
    #define POP_OPRT_INCR     0x40D
    #define POP_OPRT_INIR     0x40E
#define POP_CMD_LIST_PARA   5
    #define POP_PARA_PR       0x50F
    #define POP_PARA_SON      0x510
    #define POP_PARA_SOFF     0x511
    #define POP_PARA_MS       0x512
#define POP_CMD_LIST_CACL   6
    #define POP_CALC_SET      0x613
    #define POP_CALC_ADD      0x614
    #define POP_CALC_SUB      0x615

#endif /* POP_H */
