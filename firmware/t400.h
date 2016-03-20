// Hardware definitions


#define DISPLAY_HEIGHT  64    // Height of the display
#define CHARACTER_SPACING 5   // Width of a character+space to next character


// Pin definitions
#define BUTTON_B_PIN         7
#define BUTTON_A_PIN         1
// SDA                       2
// SCL                       3
#define VBAT_EN              4  // Enable VBAT_SENSE
#define SD_CS                5
#define BATT_STAT            A7    // Battery status pin
#define RTC_INT              0
#define BUTTON_C_PIN         8
#define BUTTON_D_PIN         9
#define BUTTON_E_PIN        10
#define PWR_ONOFF_PIN       30   // Power on/off pin turns board power off (active high)
#define VBAT_SENSE          A11  // Battery voltage /2 (D12)
// #define FLASH_CS            13
// MISO                     14
// SCK                      15
// MOSI                     16
#define BUTTON_POWER_PIN    11


#define RADIO_CE            A0       // D12
#define RADIO_CSN           A1       // D11

#define LCD_A0              A2
#define LCD_RST             A3
#define LCD_CS              A4
#define LCD_BACKLIGHT_PIN   A5   // LCD backlight on pin


#define LCD_CONTRAST     0x018*7  // Sets the LCD contrast


// Data packet format:
// 0: Command (0: hello, 1: start LED timer)
// 1: On time, in minutes (1-60)
// 2: Sleep time, in seconds (1-10)
// 3: Brightness (0-255)
// 4,5: Reserved
// 6: CRC

#define COMMAND_LENGTH 7

#define COMMAND_HELLO   0
#define COMMAND_SQUACK  1
