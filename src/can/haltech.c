#include <haltech.h>

uint16_t haltech_can_data(canMessage  *message){
    uint16_t result = 0;
    switch(message->ID){
        case 0x360:

            // uint8_t -> 8 bits
            // uint16_t -> 16 bits
            //uint32_t -> 32 bits 
            // uint64_t -> 64 bits
            //int -> 4 bytes
            uint16_t rpm = (message->DATA[0] << 8 | message->DATA[1]);
            printf("RPM: %d\n", rpm);
            uint16_t manifold_pressure_raw = (((message->DATA[2] << 8) | (message->DATA[3])));
            float manifold_pressure = (float)manifold_pressure_raw/10;
            printf("Manifold Pressure: %.2f\n", (manifold_pressure));
            uint16_t throttle_position = (((message->DATA[4] << 8) | (message->DATA[5])));
            printf("Throttle Position: %d\n", (throttle_position/10));
            break;
        case 0x362:
            uint16_t ignition_angle = (((message->DATA[4] << 8) | (message->DATA[5])));
            printf("Ignition Angle: %d\n", (ignition_angle/10));
            break;
        case 0x36B:
            uint16_t brake_pressure = (((message->DATA[0] << 8) | (message->DATA[1])));
            printf("Brake Pressure Front: %d\n", brake_pressure);
            break;
        case 0x372:
            uint16_t battery_voltage_raw = (((message->DATA[0] << 8) | (message->DATA[1])));
            float battery_voltage = (float)battery_voltage_raw/10;
            printf("Battery Voltage: %.2f\n", (battery_voltage));
            break;
        case 0x3E0:
            uint16_t coolant_temperature_raw = (((message->DATA[2] << 8) | (message->DATA[3])));
            float coolant_temperature = (float)coolant_temperature_raw/10;
            printf("Coolant Temperature: %.2f\n", (coolant_temperature));
            break;
        case 0x470:
            result = (message->DATA[7]);
            printf("Gear: %d\n", result);
            break;
        
            

    }
}