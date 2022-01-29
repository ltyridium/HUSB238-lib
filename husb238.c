#include "husb238.h"

#include "i2c.h"
#include "usart.h"

void HUSB238_ReadReg(hhusb238_t *hhusb238, uint8_t reg, uint8_t *data, uint8_t size)
{
  HAL_I2C_Mem_Read(hhusb238->i2c_handle,
                   HUSB238_I2CAddress << 1,
                   reg,
                   I2C_MEMADD_SIZE_8BIT,
                   data,
                   size,
                   HAL_MAX_DELAY);
}

void HUSB238_WriteReg(hhusb238_t *hhusb238, uint8_t reg, uint8_t *data, uint8_t size)
{
  HAL_I2C_Mem_Write(hhusb238->i2c_handle,
                    HUSB238_I2CAddress << 1,
                    reg,
                    I2C_MEMADD_SIZE_8BIT,
                    data,
                    size,
                    HAL_MAX_DELAY);
}

void HUSB238_ReadAllReg(hhusb238_t *hhusb238)
{
  HUSB238_ReadReg(hhusb238, 0x00, (uint8_t *) &hhusb238->reg, 10);
}

void HUSB238_RequestPDO(hhusb238_t *hhusb238)
{
  uint8_t cmd = Request_PDO;
  HUSB238_WriteReg(hhusb238, Reg_GO_COMMAND, &cmd, 1);
}

void HUSB238_GetSrcCap(hhusb238_t *hhusb238)
{
  uint8_t cmd = Get_SRC_Cap;
  HUSB238_WriteReg(hhusb238, Reg_GO_COMMAND, &cmd, 1);
}

void HUSB238_HardReset(hhusb238_t *hhusb238)
{
  uint8_t cmd = Hard_Reset;
  HUSB238_WriteReg(hhusb238, Reg_GO_COMMAND, &cmd, 1);
}

void HUSB238_Init(hhusb238_t *hhusb238, void *hi2c)
{
  hhusb238->i2c_handle = hi2c;
//  HUSB238_HardReset(hhusb238);
//  HAL_Delay(2000);
  HUSB238_GetSrcCap(hhusb238);
}

void HUSB238_SelVoltage(hhusb238_t *hhusb238, HUSB238_SELECT_Voltage_e voltage)
{
  hhusb238->reg.SRC_PDO.bit.PDO_SELECT = voltage;
  HUSB238_WriteReg(hhusb238, Reg_SRC_PDO_SEL, &hhusb238->reg.SRC_PDO.all, 1);
  HUSB238_RequestPDO(hhusb238);
}


float bit2current(uint8_t x)
{
  switch (x & 0xf)
  {
    case PD_0_5A:
      return 0.5f;
    case PD_0_7A:
      return 0.7f;
    case PD_1_0A:
      return 1.0f;
    case PD_1_25A:
      return 1.25f;
    case PD_1_5A:
      return 1.5f;
    case PD_1_75A:
      return 1.75f;
    case PD_2A:
      return 2.0f;
    case PD_2_25A:
      return 2.25f;
    case PD_2_5A:
      return 2.5f;
    case PD_2_75A:
      return 2.75f;
    case PD_3A:
      return 3.0f;
    case PD_3_25A:
      return 3.25f;
    case PD_3_5A:
      return 3.5f;
    case PD_4A:
      return 4.0f;
    case PD_4_5A:
      return 4.5f;
    case PD_5A:
      return 5.0f;
  }
  return -1.0f;
}

void HUSB238_PrintCap(hhusb238_t *hhusb238)
{
  for (int i = 0; i < 6; ++i)
  {
    uint8_t cap = *((uint8_t *) &hhusb238->reg.SRC_PDO_5V.all + i);
    uint8_t voltage = 0;
    switch (i)
    {
      case 0:
        voltage = 5;
        break;
      case 1:
        voltage = 9;
        break;
      case 2:
        voltage = 12;
        break;
      case 3:
        voltage = 15;
        break;
      case 4:
        voltage = 18;
        break;
      case 5:
        voltage = 20;
        break;
    }

    UART_printf(&hlpuart1, "%2uV ", voltage);
    if (cap & 0x80)
    {
      float current = bit2current(cap);
      UART_printf(&hlpuart1, "%.2fA\n", current);
    } else
    {
      UART_printf(&hlpuart1, "not support\n");
    }
  }
}

void HUSB238_GetEstd(hhusb238_t *hhusb238)
{
  HUSB238_ReadReg(hhusb238, 0x00, (uint8_t *) &hhusb238->reg, 1);
  hhusb238->EstdI = bit2current(hhusb238->reg.PD_STATUS0.bit.PD_SRC_CURRENT);
  switch (hhusb238->reg.PD_STATUS0.bit.PD_SRC_VOLTAGE)
  {
    case PDO_5V:
      hhusb238->EstdU = 5.0f;
      break;
    case PDO_9V:
      hhusb238->EstdU = 9.0f;
      break;
    case PDO_12V:
      hhusb238->EstdU = 12.0f;
      break;
    case PDO_15V:
      hhusb238->EstdU = 15.0f;
      break;
    case PDO_18V:
      hhusb238->EstdU = 18.0f;
      break;
    case PDO_20V:
      hhusb238->EstdU = 20.0f;
      break;
    default:
      hhusb238->EstdU = 0.0f;
      break;
  }
}

