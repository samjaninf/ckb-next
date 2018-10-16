#include "led.h"
#include "usb.h"

int updatergb_wireless(usbdevice* kb, lighting* lastlight, lighting* newlight){
    // Dark Core zone bits:
    // 1: Front
    // 2: Thumb
    // 4: Rear

    // Dark Core commands:
    // 00: Colour Shift
    // 01: Colour Pulse
    // 03: Rainbow
    // 07: Static Colour
    // FF: No animation (black)

    int max_zones = 1;
    int bitmask = 7;
    int profile_byte = 0;

    int red = newlight->r[0];
    int green = newlight->r[0];
    int blue = newlight->r[0];

    // to iterate through all three zones on the DARK CORE increase the loop limit
    if (IS_DARK_CORE(kb)) {
        max_zones = 3;
    }

    for (int zone = 0; zone < max_zones; zone++) {
        // set variable bits for DARK CORE mouse based on zone
        if (IS_DARK_CORE(kb)) {
            bitmask = 1 << zone;
            profile_byte = 5 - zone;

            red = newlight->r[zone];
            green = newlight->g[zone];
            blue = newlight->b[zone];
        }

        uchar data_pkt[MSG_SIZE] = {
            CMD_SET, 0xaa, 0
        };
        data_pkt[4]  = bitmask;
        data_pkt[5]  = 7;                // Command (static colour).
        data_pkt[8]  = 100;              // Opacity (100%).
        data_pkt[9]  = red;
        data_pkt[10] = green;
        data_pkt[11] = blue;
        // Colour gets sent twice for some reason.
        data_pkt[12] = red;
        data_pkt[13] = green;
        data_pkt[14] = blue;
        data_pkt[15] = profile_byte;

        if(!usbsend(kb, data_pkt, 1))
            return -1;
    }

    memcpy(lastlight, newlight, sizeof(lighting));
    return 0;
}
