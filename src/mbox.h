#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000

#define MBOX_REQUEST    0

#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LEDS    4
#define MBOX_CH_BTNS    5
#define MBOX_CH_TOUCH   6
#define MBOX_CH_COUNT   7
#define MBOX_CH_PROP    8

typedef enum
{
	MBOX_TAG_LAST =                                       0,

	MBOX_TAG_GET_FIRMWARE_REVISION =                  0x00000001,
	MBOX_TAG_GET_FIRMWARE_VARIANT =                   0x00000002,
	MBOX_TAG_GET_FIRMWARE_HASH =                      0x00000003,

	MBOX_TAG_SET_CURSOR_INFO =                        0x00008010,
	MBOX_TAG_SET_CURSOR_STATE =                       0x00008011,

	MBOX_TAG_GET_BOARD_MODEL =                        0x00010001,
	MBOX_TAG_GET_BOARD_REVISION =                     0x00010002,
	MBOX_TAG_GET_BOARD_MAC_ADDRESS =                  0x00010003,
	MBOX_TAG_GET_BOARD_SERIAL =                       0x00010004,
	MBOX_TAG_GET_ARM_MEMORY =                         0x00010005,
	MBOX_TAG_GET_VC_MEMORY =                          0x00010006,
	MBOX_TAG_GET_CLOCKS =                             0x00010007,
	MBOX_TAG_GET_POWER_STATE =                        0x00020001,
	MBOX_TAG_GET_TIMING =                             0x00020002,
	MBOX_TAG_SET_POWER_STATE =                        0x00028001,
	MBOX_TAG_GET_CLOCK_STATE =                        0x00030001,
	MBOX_TAG_GET_CLOCK_RATE =                         0x00030002,
	MBOX_TAG_GET_VOLTAGE =                            0x00030003,
	MBOX_TAG_GET_MAX_CLOCK_RATE =                     0x00030004,
	MBOX_TAG_GET_MAX_VOLTAGE =                        0x00030005,
	MBOX_TAG_GET_TEMPERATURE =                        0x00030006,
	MBOX_TAG_GET_MIN_CLOCK_RATE =                     0x00030007,
	MBOX_TAG_GET_MIN_VOLTAGE =                        0x00030008,
	MBOX_TAG_GET_TURBO =                              0x00030009,
	MBOX_TAG_GET_MAX_TEMPERATURE =                    0x0003000a,
	MBOX_TAG_GET_STC =                                0x0003000b,
	MBOX_TAG_ALLOCATE_MEMORY =                        0x0003000c,
	MBOX_TAG_LOCK_MEMORY =                            0x0003000d,
	MBOX_TAG_UNLOCK_MEMORY =                          0x0003000e,
	MBOX_TAG_RELEASE_MEMORY =                         0x0003000f,
	MBOX_TAG_EXECUTE_CODE =                           0x00030010,
	MBOX_TAG_EXECUTE_QPU =                            0x00030011,
	MBOX_TAG_SET_ENABLE_QPU =                         0x00030012,
	MBOX_TAG_GET_DISPMANX_RESOURCE_MEM_HANDLE =       0x00030014,
	MBOX_TAG_GET_EDID_BLOCK =                         0x00030020,
	MBOX_TAG_GET_CUSTOMER_OTP =                       0x00030021,
	MBOX_TAG_GET_EDID_BLOCK_DISPLAY =                 0x00030023,
	MBOX_TAG_GET_DOMAIN_STATE =                       0x00030030,
	MBOX_TAG_GET_THROTTLED =                          0x00030046,
	MBOX_TAG_GET_CLOCK_RATE_MEASURED =                0x00030047,
	MBOX_TAG_NOTIFY_REBOOT =                          0x00030048,
	MBOX_TAG_SET_CLOCK_STATE =                        0x00038001,
	MBOX_TAG_SET_CLOCK_RATE =                         0x00038002,
	MBOX_TAG_SET_VOLTAGE =                            0x00038003,
	MBOX_TAG_SET_TURBO =                              0x00038009,
	MBOX_TAG_SET_CUSTOMER_OTP =                       0x00038021,
	MBOX_TAG_SET_DOMAIN_STATE =                       0x00038030,
	MBOX_TAG_GET_GPIO_STATE =                         0x00030041,
	MBOX_TAG_SET_GPIO_STATE =                         0x00038041,
	MBOX_TAG_SET_SDHOST_CLOCK =                       0x00038042,
	MBOX_TAG_GET_GPIO_CONFIG =                        0x00030043,
	MBOX_TAG_SET_GPIO_CONFIG =                        0x00038043,
	MBOX_TAG_GET_PERIPH_REG =                         0x00030045,
	MBOX_TAG_SET_PERIPH_REG =                         0x00038045,
	MBOX_TAG_GET_POE_HAT_VAL =                        0x00030049,
	MBOX_TAG_SET_POE_HAT_VAL =                        0x00038049,
	MBOX_TAG_SET_POE_HAT_VAL_OLD =                    0x00030050,
	MBOX_TAG_NOTIFY_XHCI_RESET =                      0x00030058,
	MBOX_TAG_GET_REBOOT_FLAGS =                       0x00030064,
	MBOX_TAG_SET_REBOOT_FLAGS =                       0x00038064,
	MBOX_TAG_NOTIFY_DISPLAY_DONE =                    0x00030066,
	MBOX_TAG_GET_BUTTONS_PRESSED =                    0x00030088,

	/* Dispmanx TAGS */
	MBOX_TAG_FRAMEBUFFER_ALLOCATE =                   0x00040001,
	MBOX_TAG_FRAMEBUFFER_BLANK =                      0x00040002,
	MBOX_TAG_FRAMEBUFFER_GET_PHYSICAL_WIDTH_HEIGHT =  0x00040003,
	MBOX_TAG_FRAMEBUFFER_GET_VIRTUAL_WIDTH_HEIGHT =   0x00040004,
	MBOX_TAG_FRAMEBUFFER_GET_DEPTH =                  0x00040005,
	MBOX_TAG_FRAMEBUFFER_GET_PIXEL_ORDER =            0x00040006,
	MBOX_TAG_FRAMEBUFFER_GET_ALPHA_MODE =             0x00040007,
	MBOX_TAG_FRAMEBUFFER_GET_PITCH =                  0x00040008,
	MBOX_TAG_FRAMEBUFFER_GET_VIRTUAL_OFFSET =         0x00040009,
	MBOX_TAG_FRAMEBUFFER_GET_OVERSCAN =               0x0004000a,
	MBOX_TAG_FRAMEBUFFER_GET_PALETTE =                0x0004000b,
	MBOX_TAG_FRAMEBUFFER_GET_LAYER =                  0x0004000c,
	MBOX_TAG_FRAMEBUFFER_GET_TRANSFORM =              0x0004000d,
	MBOX_TAG_FRAMEBUFFER_GET_VSYNC =                  0x0004000e,
	MBOX_TAG_FRAMEBUFFER_GET_TOUCHBUF =               0x0004000f,
	MBOX_TAG_FRAMEBUFFER_GET_GPIOVIRTBUF =            0x00040010,
	MBOX_TAG_FRAMEBUFFER_RELEASE =                    0x00048001,
	MBOX_TAG_FRAMEBUFFER_GET_DISPLAY_ID =             0x00040016,
	MBOX_TAG_FRAMEBUFFER_SET_DISPLAY_NUM =            0x00048013,
	MBOX_TAG_FRAMEBUFFER_GET_NUM_DISPLAYS =           0x00040013,
	MBOX_TAG_FRAMEBUFFER_GET_DISPLAY_SETTINGS =       0x00040014,
	MBOX_TAG_FRAMEBUFFER_TEST_PHYSICAL_WIDTH_HEIGHT = 0x00044003,
	MBOX_TAG_FRAMEBUFFER_TEST_VIRTUAL_WIDTH_HEIGHT =  0x00044004,
	MBOX_TAG_FRAMEBUFFER_TEST_DEPTH =                 0x00044005,
	MBOX_TAG_FRAMEBUFFER_TEST_PIXEL_ORDER =           0x00044006,
	MBOX_TAG_FRAMEBUFFER_TEST_ALPHA_MODE =            0x00044007,
	MBOX_TAG_FRAMEBUFFER_TEST_VIRTUAL_OFFSET =        0x00044009,
	MBOX_TAG_FRAMEBUFFER_TEST_OVERSCAN =              0x0004400a,
	MBOX_TAG_FRAMEBUFFER_TEST_PALETTE =               0x0004400b,
	MBOX_TAG_FRAMEBUFFER_TEST_LAYER =                 0x0004400c,
	MBOX_TAG_FRAMEBUFFER_TEST_TRANSFORM =             0x0004400d,
	MBOX_TAG_FRAMEBUFFER_TEST_VSYNC =                 0x0004400e,
	MBOX_TAG_FRAMEBUFFER_SET_PHYSICAL_WIDTH_HEIGHT =  0x00048003,
	MBOX_TAG_FRAMEBUFFER_SET_VIRTUAL_WIDTH_HEIGHT =   0x00048004,
	MBOX_TAG_FRAMEBUFFER_SET_DEPTH =                  0x00048005,
	MBOX_TAG_FRAMEBUFFER_SET_PIXEL_ORDER =            0x00048006,
	MBOX_TAG_FRAMEBUFFER_SET_ALPHA_MODE =             0x00048007,
	MBOX_TAG_FRAMEBUFFER_SET_PITCH =                  0x00048008,
	MBOX_TAG_FRAMEBUFFER_SET_VIRTUAL_OFFSET =         0x00048009,
	MBOX_TAG_FRAMEBUFFER_SET_OVERSCAN =               0x0004800a,
	MBOX_TAG_FRAMEBUFFER_SET_PALETTE =                0x0004800b,

	MBOX_TAG_FRAMEBUFFER_SET_TOUCHBUF =               0x0004801f,
	MBOX_TAG_FRAMEBUFFER_SET_GPIOVIRTBUF =            0x00048020,
	MBOX_TAG_FRAMEBUFFER_SET_VSYNC =                  0x0004800e,
	MBOX_TAG_FRAMEBUFFER_SET_LAYER =                  0x0004800c,
	MBOX_TAG_FRAMEBUFFER_SET_TRANSFORM =              0x0004800d,
	MBOX_TAG_FRAMEBUFFER_SET_BACKLIGHT =              0x0004800f,

	MBOX_TAG_VCHIQ_INIT =                             0x00048010,

	MBOX_TAG_SET_PLANE =                              0x00048015,
	MBOX_TAG_GET_DISPLAY_TIMING =                     0x00040017,
	MBOX_TAG_SET_TIMING =                             0x00048017,
	MBOX_TAG_GET_DISPLAY_CFG =                        0x00040018,
	MBOX_TAG_SET_DISPLAY_POWER =		      		  0x00048019,
	MBOX_TAG_GET_COMMAND_LINE =                       0x00050001,
	MBOX_TAG_GET_DMA_CHANNELS =                       0x00060001,
} mbox_tag;

#define MBOX_CLOCK_reserved     0x000000000
#define MBOX_CLOCK_EMMC         0x000000001
#define MBOX_CLOCK_UART         0x000000002
#define MBOX_CLOCK_ARM          0x000000003
#define MBOX_CLOCK_CORE         0x000000004
#define MBOX_CLOCK_V3D          0x000000005
#define MBOX_CLOCK_H264         0x000000006
#define MBOX_CLOCK_ISP          0x000000007
#define MBOX_CLOCK_SDRAM        0x000000008
#define MBOX_CLOCK_PIXEL        0x000000009
#define MBOX_CLOCK_PWM          0x00000000a
#define MBOX_CLOCK_HEVC         0x00000000b
#define MBOX_CLOCK_EMMC2        0x00000000c
#define MBOX_CLOCK_M2MC         0x00000000d
#define MBOX_CLOCK_PIXEL_BVB    0x00000000e

static const char* MboxClockNames[] =
{
	"reserved",
	"EMMC",
	"UART",
	"ARM",
	"CORE",
	"V3D",
	"H264",
	"ISP",
	"SDRAM",
	"PIXEL",
	"PWM",
	"HEVC",
	"EMMC2",
	"M2MC",
	"PIXEL_BVB",
};

#define MBOX_GPIO_STATUS_LED 42
#define MBOX_GPIO_POWER_LED  130

#define MBOX_MEM_FLAG_DISCARDABLE       (1 << 0) /* can be resized to 0 at any time. Use for cached data */
#define MBOX_MEM_FLAG_NORMAL            (0 << 2) /* normal allocating alias. Don't use from ARM */
#define MBOX_MEM_FLAG_DIRECT            (1 << 2) /* 0xC alias uncached */
#define MBOX_MEM_FLAG_COHERENT          (2 << 2) /* 0x8 alias. Non-allocating in L2 but coherent */
#define MBOX_MEM_FLAG_L1_NONALLOCATING  ((MEM_FLAG_DIRECT | MEM_FLAG_COHERENT)) /* Allocating in L2 */
#define MBOX_MEM_FLAG_ZERO              (1 << 4)  /* initialise buffer to all zeros */
#define MBOX_MEM_FLAG_NO_INIT           (1 << 5) /* don't initialise (default is initialise to all ones */
#define MBOX_MEM_FLAG_HINT_PERMALOCK    (1 << 6) /* Likely to be locked for long periods of time. */
