// HW01: HỆ THỐNG GIÁM SÁT LÒ NHIỆT CÔNG NGHIỆP

#include <stdio.h>
#include <stdint.h>

// TASK 1: POINTERS & MEMORY

void parse_config(const uint8_t *config_packet, int16_t *high_threshold) {

    if (config_packet == NULL || high_threshold == NULL) {
        return;
    }

    *high_threshold = (int16_t)(
        ((int16_t)config_packet[1] << 8) | //dịch sang trái 8 bit và ghép với low
        config_packet[0]
        );
}

// TASK 2: COMPILER & VOLATILE

int16_t read_temperature_reg(void *hw_sensor_reg) {
    if (hw_sensor_reg == NULL) {
        return 0;
    }

    volatile int16_t* sensor = (volatile int16_t*)hw_sensor_reg;  //ép kiểu cho con trỏ hw_sensor_reg

    return *sensor;
}

// TASK 3: DATA TYPES & BITWISE OPERATIONS

void control_output(uint8_t *control_reg, uint8_t fan_enable, uint8_t alarm_enable) {

    if (control_reg == NULL) {
        return;
    }

    // Điều khiển Fan (Bit 0)
    if (fan_enable) {
        *control_reg |= (1 << 0);
    }
    else {
        *control_reg &= ~(1 << 0); //đảo bit
    }

    // Điều khiển Alarm (Bit 1)
    if (alarm_enable) {
        *control_reg |= (1 << 1);
    }
    else {
        *control_reg &= ~(1 << 1);
    }

}

// HÀM MAIN KIỂM TRA (Học viên giữ nguyên để chạy thử nghiệm, chỉ thay đổi input nếu cần)

int main() {

    // 1. Test Task 1
    int16_t threshold = 0;
    uint8_t mock_packet[2] = {0x64, 0x00};
    parse_config(mock_packet, &threshold); 
    printf("Threshold Parsed: %d C\n", threshold);

    // 2. Test Task 2
    volatile int16_t mock_hardware_sensor = 105; 
    int16_t current_temp = read_temperature_reg((void*)&mock_hardware_sensor);
    printf("Current Temp Read: %d C\n", current_temp);

    // 3. Test Task 3
    uint8_t system_control_register = 0xFC; // 1111 1100
    if (current_temp >= threshold) {
        control_output(&system_control_register, 1, 0);
    }
    printf("Control Reg Output: 0x%02X\n", system_control_register);

    return 0;
}