/* CH573F TFT��Ļ�������Գ���
 * ѩ�� ��д   https://github.com/2827700630
 * ����޸����� 2025-10-2
 * ������CH573F��Ƭ������дʱ�ο����ߺ��SPI0ʾ������ʹ��Ӳ��SPI0��DMA����
 * ֧��ST7735S��ST7789v3��Ļ����ֻ��Ҫ����Ļ����ȥ���У���������ST7735S��ST7789v3������Ļ
 * ���߷�ʽ��
 * GND      -> GND
 * VCC      -> 3.3V
 * SCL(SCK) -> PA13 (SPI0_SCK)
 * SDA(MOSI)-> PA14 (SPI0_MOSI)
 * TFT_CS   -> PA12
 * TFT_RES  -> PA11 (Ҳ���Խӵ���Ƭ���ĸ�λ��RST)
 * TFT_DC   -> PA10
 * TFT_BLK  -> PB22 (Ҳ���԰����ӵ�3.3V)
 * ������Ϣ��ο�README.md
 */

#include "CH57x_common.h"

// ����TFT���������ͷ�ļ�
#include "TFTh/TFT_CAD.h"  // ������ͼ������ IO ����
#include "TFTh/TFT_init.h"
#include "TFTh/TFT_text.h"
#include "TFTh/TFT_io.h"  // ����RGBת������

// TFT��Ļ�����SPI�������
TFT_HandleTypeDef htft1;  // ��һ��TFT��Ļ����
SPI_HandleTypeDef SPI0;   // ��һ��TFT��Ļ����
// TFT��Ļ���Ŷ��壬����ʵ�������޸�
#define TFT_CS_GPIO_Port 0  // 0��GPIOA��1��GPIOB
#define TFT_CS_Pin GPIO_Pin_12
#define TFT_DC_GPIO_Port 0
#define TFT_DC_Pin GPIO_Pin_10
#define TFT_RES_GPIO_Port 0
#define TFT_RES_Pin GPIO_Pin_11
#define TFT_BLK_GPIO_Port 1
#define TFT_BLK_Pin GPIO_Pin_22
// SPI0���Ŷ�����TFTio.c�ļ��е�SPI0_MasterDefInit����֮ǰ��CH573Fֻ��һ��Ӳ��SPI
// ��Ļ�ֱ��ʶ��壬����ʵ����Ļ�޸�
int HEIGHT = 320;
int WIDTH = 240;

/**
 * @fn     main
 * @brief   ������
 * @return  none
 */
int main() {
    SetSysClock (CLK_SOURCE_PLL_60MHz);
    int i = 0;  // ������

    /* ���ô��ڵ��� */
    GPIOA_SetBits (GPIO_Pin_9);
    GPIOA_ModeCfg (GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg (GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
    PRINT ("Start @ChipID=%02X\n", R8_CHIP_ID);
    /* ������Ļ���ţ��ߺ�ĵ�Ƭ��GPIOA��GPIOB���������Ƿֿ��ġ��������ò�Ȼ��Ļ�޷���ʾ */
    GPIOA_ModeCfg (TFT_CS_Pin, GPIO_ModeOut_PP_5mA);
    GPIOA_ModeCfg (TFT_RES_Pin, GPIO_ModeOut_PP_5mA);
    GPIOA_ModeCfg (TFT_DC_Pin, GPIO_ModeOut_PP_5mA);
    GPIOB_ModeCfg (TFT_BLK_Pin, GPIO_ModeOut_PP_5mA);
    /* ��Ļ���� */
    TFT_Init_Instance (&htft1, &SPI0, TFT_CS_GPIO_Port, TFT_CS_Pin);
    TFT_Config_Pins (&htft1, TFT_DC_GPIO_Port, TFT_DC_Pin, TFT_RES_GPIO_Port, TFT_RES_Pin, TFT_BLK_GPIO_Port, TFT_BLK_Pin);  // �����ֶ���������
                                                                                                                             // ���÷���X/Yƫ��

    while (1)                                                                                                                // ѭ��
    {
        i = i + 1;
        if (i % 2 == 0) {
            TFT_Config_Display (&htft1, 0, 0, 0);
            TFT_Init_ST7789v3 (&htft1);  // ST7789v3��Ļ��ʼ��,�Ҽ�����tft_init.c�鿴������Ļ�ĳ�ʼ������
            TFT_Show_String (&htft1, 20, 20, "now ST7789v3", WHITE, BLACK, 16, 0);
            HEIGHT = 320;
            WIDTH = 240;
        } else {
            TFT_Config_Display (&htft1, 2, 2, 1);
            TFT_Init_ST7735S (&htft1);  // ST7735S��Ļ��ʼ��,�Ҽ�����tft_init.c�鿴������Ļ�ĳ�ʼ������
            TFT_Show_String (&htft1, 20, 20, "now ST7735S", WHITE, BLACK, 16, 0);
            HEIGHT = 160;
            WIDTH = 128;
        }

        /* ������ɫ��䣬��������ɫ��ʾ�Ƿ���ȷ */
        TFT_Fill_Area (&htft1, 0, 40, WIDTH, HEIGHT, RED);
        TFT_Fill_Area (&htft1, 0, 40, WIDTH, HEIGHT, GREEN);
        TFT_Fill_Area (&htft1, 0, 40, WIDTH, HEIGHT, BLUE);
        /* Ӣ����ʾ */
        TFT_Show_String (&htft1, 20, 40, "Font 16", WHITE, BLACK, 16, 0);
        TFT_Show_String (&htft1, 20, 60, "Font 16", BLACK, WHITE, 16, 0);
        /* ��ͼ���������ŵ���������ע�� */
        TFT_Draw_Fast_VLine (&htft1, 10, 40, HEIGHT, WHITE);
        TFT_Draw_Rectangle (&htft1, 30, 90, WIDTH - 70, HEIGHT - 70, BLUE);
        TFT_Fill_Rectangle (&htft1, 30, 160, WIDTH - 20, HEIGHT - 20, GREEN);
        TFT_Draw_Line (&htft1, 0, 40, WIDTH - 1, HEIGHT - 1, TFT_RGB (20, 60, 200));
        TFT_Draw_Circle (&htft1, WIDTH / 2, HEIGHT / 2, 30, BRRED);
        TFT_Fill_Circle (&htft1, WIDTH / 3, HEIGHT / 3, 20, CYAN);
        TFT_Draw_Ellipse (&htft1, WIDTH / 4, HEIGHT / 4, 60, 30, WHITE);
        TFT_Fill_Ellipse (&htft1, WIDTH - 50, HEIGHT - 50, 50, 20, GBLUE);
        DelayMs (2000);
        TFT_Fill_Area (&htft1, 0, 00, WIDTH, HEIGHT, BLACK);  // ����
        PRINT ("loop %d\n", i);                               // �����ʹ�ô������ֵ�ʱ������ܣ�����ÿ��ѭ����ʱ��
    }
}
