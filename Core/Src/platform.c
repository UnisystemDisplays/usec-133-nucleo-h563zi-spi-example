#include <string.h>
#include "platform.h"
#include "spi.h"

/* macro */
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))


/*
 * platform_init()
 */
uint8_t
platform_init (usec_ctx *ctx)
{
  HAL_GPIO_WritePin (EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin (EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET);

  /* init platform specific data - not used */
  ctx->platform = NULL;

  return USEC_DEV_OK;
}

/*
 * platform_deinit()
 */
uint8_t
platform_deinit (usec_ctx *ctx)
{
  return USEC_DEV_OK;
}

/*
 * platform_delay_ms()
 */
uint8_t
platform_delay_ms (void      *platform,
                   uint16_t   delay_ms)
{
  HAL_Delay (delay_ms);

  return USEC_DEV_OK;
}

/*
 * platform_hw_reset()
 */
uint8_t
platform_hw_reset (void *platform)
{
  HAL_GPIO_WritePin (EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET);
  platform_delay_ms (platform, 100);
  HAL_GPIO_WritePin (EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_SET);
  platform_delay_ms (platform, 200);

  return USEC_DEV_OK;
}

/*
 * platform_spi_write_byte()
 */
uint8_t
platform_spi_write_byte (void    *platform,
                         uint8_t  val)
{
  HAL_SPI_Transmit (&hspi1, &val, 1, HAL_MAX_DELAY);
  return USEC_DEV_OK;
}

/*
 * platform_spi_write_bytes()
 */
uint8_t
platform_spi_write_bytes (void      *platform,
                          uint8_t   *data,
                          uint32_t   len)
{
  uint32_t transfer_left;
  uint32_t transfer_size;

  transfer_left = len;

  while (1)
    {
      /* transfer size */
      transfer_size = MIN(transfer_left, 65536-1);
      HAL_SPI_Transmit (&hspi1, data, transfer_size, HAL_MAX_DELAY);

      transfer_left -= transfer_size;
      data += transfer_size;

      if (transfer_left == 0)
      break;
    }

  return USEC_DEV_OK;
}

/*
 * platform_spi_read_byte()
 */
uint8_t
platform_spi_read_byte (void *platform)
{
  uint8_t val;

  HAL_SPI_Receive (&hspi1, &val, 1, HAL_MAX_DELAY);

  return val;
}

/*
 * platform_spi_cs_high()
 */
uint8_t
platform_spi_cs_high (void     *platform,
                      uint8_t   cs_num)
{
  HAL_GPIO_WritePin (EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET);
  return USEC_DEV_OK;
}

/*
 * platform_spi_cs_low()
 */
uint8_t
platform_spi_cs_low (void     *platform,
                     uint8_t   cs_num)
{
  HAL_GPIO_WritePin (EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);
  return USEC_DEV_OK;
}

/*
 * platform_gpio_read()
 */
uint8_t
platform_gpio_read (void *platform)
{
  return HAL_GPIO_ReadPin (EPD_RDY_GPIO_Port, EPD_RDY_Pin);
}
