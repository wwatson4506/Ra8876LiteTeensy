//=============================================================================
// Default flexio pins - Can be setup for different teensy 4.x boards
//=============================================================================

#if defined(ARDUINO_TEENSY41)
// FlexIO pins: data: 19 18 14 15 40 41 17 16 WR:36 RD:37
//  FlexIO3
#define DISPLAY_WR 36
#define DISPLAY_RD 37

#define DISPLAY_D0 19	// FlexIO3:0 D0
#define DISPLAY_D1 18	// FlexIO3:1 |
#define DISPLAY_D2 14	// FlexIO3:2 |
#define DISPLAY_D3 15	// FlexIO3:3 |
#define DISPLAY_D4 40	// FlexIO3:4 |
#define DISPLAY_D5 41	// FlexIO3:5 |
#define DISPLAY_D6 17	// FlexIO3:6 |
#define DISPLAY_D7 16	// FlexIO3:7 D7

#define DISPLAY_D8 22	// FlexIO3:8 D8
#define DISPLAY_D9 23	// FlexIO3:9  |
#define DISPLAY_D10 20	// FlexIO3:10 |
#define DISPLAY_D11 21	// FlexIO3:11 |
#define DISPLAY_D12 38	// FlexIO3:12 |
#define DISPLAY_D13 39	// FlexIO3:13 |
#define DISPLAY_D14 26	// FlexIO3:14 |
#define DISPLAY_D15 27	// FlexIO3:15 D15

#elif defined(ARDUINO_TEENSY40)
// BUGBUG Nibble mode
// Will currently fail
#define DISPLAY_RD 20	// FlexIO3:10: RD
#define DISPLAY_WR 21	// FlexIO3:11 WR

#define DISPLAY_D0 19	 // FlexIO3:0 D0
#define DISPLAY_D1 18	 // FlexIO3:1 |
#define DISPLAY_D2 14	 // FlexIO3:2 |
#define DISPLAY_D3 15	 // FlexIO3:3 |
#define DISPLAY_D4 17	 // FlexIO3:6 |
#define DISPLAY_D5 16	 // FlexIO3:7 |
#define DISPLAY_D6 22	 // FlexIO3:8 |
#define DISPLAY_D7 23	 // FlexIO3:9 D7

#else // #if defined(ARDUINO_TEENSY_MICROMOD)
// FLEXIO2 pins.

#define DISPLAY_WR 7  // FLEXIO2:17
#define DISPLAY_RD 8 // FLEXIO2:16

#define DISPLAY_D0 40 // 40      B0_04   FlexIO2:4
#define DISPLAY_D1 41 // 41      B0_05   FlexIO2:5
#define DISPLAY_D2 42 // 42      B0_06   FlexIO2:6
#define DISPLAY_D3 43 // 43      B0_07   FlexIO2:7
#define DISPLAY_D4 44 // 44      B0_08   FlexIO2:8
#define DISPLAY_D5 45 // 45      B0_09   FlexIO2:9
#define DISPLAY_D6 6  // 6       B0_10   FlexIO2:10
#define DISPLAY_D7 9  // 9       B0_11   FlexIO2:11
#endif