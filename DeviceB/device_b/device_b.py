import random
import time

import serial
import serial.threaded

from DeviceB.device_b import DeviceBSerialProtocol

DEFAULT_SERIAL_PORT = '/dev/ttyACM0'
DEFAULT_BAUDRATE = 115200
DEFAULT_LEDS_STATE_INTERVAL_MS = 250


def generate_leds_state_text() -> str:
    return 'ELEL329-LEDS:' + ''.join(
        (random.choice(('H', 'L')) for _ in range(3))) + '\n'


def main(serial_port_name=DEFAULT_SERIAL_PORT, baudrate=DEFAULT_BAUDRATE,
         leds_state_interval_ms=DEFAULT_LEDS_STATE_INTERVAL_MS):
    serial_port = None
    leds_state_interval = float(leds_state_interval_ms) / 1000.0  # secs
    try:
        serial_port = serial.Serial(serial_port_name, baudrate=baudrate)
        with serial.threaded.ReaderThread(
                serial_port, DeviceBSerialProtocol) as serial_protocol:
            assert isinstance(serial_protocol, DeviceBSerialProtocol)
            while True:
                serial_protocol.wait()
                if serial_protocol.should_send_leds_state():
                    leds_state = generate_leds_state_text()
                    # print(f"Sending: {leds_state.rstrip()}")
                    serial_protocol.write_line(leds_state)
                    time.sleep(leds_state_interval)
    except (SystemExit, KeyboardInterrupt):
        print("Closing...")
        if isinstance(serial_port, serial.Serial) and serial_port.is_open:
            serial_port.close()
    except serial.SerialException as exception:
        print(f"Problem with serial port:\n\t{exception}")


if __name__ == '__main__':
    main()
