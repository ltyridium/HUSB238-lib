#ifndef __HUSB238_H__
#define __HUSB238_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#include "main.h"


#define HUSB238_I2CAddress (0x08)


typedef enum {
    PD_0_5A = 0x00,
    PD_0_7A,
    PD_1_0A,
    PD_1_25A,
    PD_1_5A,
    PD_1_75A,
    PD_2A,
    PD_2_25A,
    PD_2_5A,
    PD_2_75A,
    PD_3A,
    PD_3_25A,
    PD_3_5A,
    PD_4A,
    PD_4_5A,
    PD_5A,
} HUSB238_CURRENT_e;

typedef enum {
    Not_sel = 0x0,
    SRC_PDO_5V = 0x1,
    SRC_PDO_9V = 0x2,
    SRC_PDO_12V = 0x3,
    SRC_PDO_15V = 0x8,
    SRC_PDO_18V = 0x9,
    SRC_PDO_20V = 0xa,
} HUSB238_SELECT_Voltage_e;

typedef enum {
    Unattached = 0x0,
    PDO_5V,
    PDO_9V,
    PDO_12V,
    PDO_15V,
    PDO_18V,
    PDO_20V,
} HUSB238_Contract_Voltage_e;


enum HUSB238_CMD {
    Request_PDO = 0b00001,
    Get_SRC_Cap = 0b00100,
    Hard_Reset = 0b10000,
};


typedef union {
    struct {
        HUSB238_CURRENT_e PD_SRC_CURRENT: 4;
        HUSB238_Contract_Voltage_e PD_SRC_VOLTAGE: 4;
    } bit;
    uint8_t all;
} HUSB238_Reg_PD_STATUS0;

typedef union {
    struct {
        uint8_t CURRENT_5V: 2;
        uint8_t VOLTAGE_5V: 1;
        uint8_t PD_RESPONSE: 3;
        uint8_t ATTACH: 1;
        uint8_t CC_DIR: 1;
    } bit;
    uint8_t all;
} HUSB238_Reg_PD_STATUS1;

typedef union {
    struct {
        HUSB238_CURRENT_e SRC_5V_CURRENT: 4;
        uint8_t RESERVED: 3;
        uint8_t SRC_5V_DETECT: 1;
    } bit;
    uint8_t all;
} HUSB238_Reg_SRC_PDO_5V;
typedef union {
    struct {
        HUSB238_CURRENT_e SRC_9V_CURRENT: 4;
        uint8_t RESERVED: 3;
        uint8_t SRC_9V_DETECT: 1;
    } bit;
    uint8_t all;
} HUSB238_Reg_SRC_PDO_9V;
typedef union {
    struct {
        HUSB238_CURRENT_e SRC_12V_CURRENT: 4;
        uint8_t RESERVED: 3;
        uint8_t SRC_12V_DETECT: 1;
    } bit;
    uint8_t all;
} HUSB238_Reg_SRC_PDO_12V;
typedef union {
    struct {
        HUSB238_CURRENT_e SRC_15V_CURRENT: 4;
        uint8_t RESERVED: 3;
        uint8_t SRC_15V_DETECT: 1;
    } bit;
    uint8_t all;
} HUSB238_Reg_SRC_PDO_15V;
typedef union {
    struct {
        HUSB238_CURRENT_e SRC_18V_CURRENT: 4;
        uint8_t RESERVED: 3;
        uint8_t SRC_18V_DETECT: 1;
    } bit;
    uint8_t all;
} HUSB238_Reg_SRC_PDO_18V;

typedef union {
    struct {
        HUSB238_CURRENT_e SRC_20V_CURRENT: 4;
        uint8_t RESERVED: 3;
        uint8_t SRC_20V_DETECT: 1;
    } bit;
    uint8_t all;
} HUSB238_Reg_SRC_PDO_20V;

typedef union {
    struct {
        uint8_t RESERVED: 4;
        HUSB238_SELECT_Voltage_e PDO_SELECT: 4;
    } bit;
    uint8_t all;
} HUSB238_Reg_SRC_PDO_SEL;

typedef struct {
    HUSB238_Reg_PD_STATUS0 PD_STATUS0;
    HUSB238_Reg_PD_STATUS1 PD_STATUS1;
    HUSB238_Reg_SRC_PDO_5V SRC_PDO_5V;
    HUSB238_Reg_SRC_PDO_9V SRC_PDO_9V;
    HUSB238_Reg_SRC_PDO_12V SRC_PDO_12V;
    HUSB238_Reg_SRC_PDO_15V SRC_PDO_15V;
    HUSB238_Reg_SRC_PDO_18V SRC_PDO_18V;
    HUSB238_Reg_SRC_PDO_20V SRC_PDO_20V;
    HUSB238_Reg_SRC_PDO_SEL SRC_PDO;
    uint8_t GO_COMMAND;
} HUSB238_reg_t;

enum HUSB238_reg_addr {
    Reg_PD_STATUS0 = 0x00,
    Reg_PD_STATUS1,
    Reg_SRC_PDO_5V,
    Reg_SRC_PDO_9V,
    Reg_SRC_PDO_12V,
    Reg_SRC_PDO_15V,
    Reg_SRC_PDO_18V,
    Reg_SRC_PDO_20V,
    Reg_SRC_PDO_SEL,
    Reg_GO_COMMAND,
};

typedef struct {
    uint8_t if_detected;
    float current_range;
} HUSB238_DetectedVoltage_t;

typedef struct {
    void *i2c_handle;
    HUSB238_reg_t reg;
    float EstdU;
    float EstdI;

} hhusb238_t;

void HUSB238_Init(hhusb238_t *hhusb238, void *hi2c);

void HUSB238_ReadAllReg(hhusb238_t *hhusb238);

void HUSB238_SelVoltage(hhusb238_t *hhusb238, HUSB238_SELECT_Voltage_e voltage);

void HUSB238_PrintCap(hhusb238_t *hhusb238);

void HUSB238_GetEstd(hhusb238_t *hhusb238);


#ifdef __cplusplus
}
#endif

#endif /* __HUSB238_H */
