import random
import time

import click as click
import serial
import serial.threaded

from device_b import DeviceBSerialProtocol

DEFAULT_SERIAL_PORT = '/dev/ttyACM0'
DEFAULT_BAUDRATE = 115200
DEFAULT_LEDS_STATE_INTERVAL_MS = 250


def generate_leds_state_text() -> str:
    return 'ELEL329-LEDS:' + ''.join(
        (random.choice(('H', 'L')) for _ in range(3))) + '\n'


@click.command()
@click.option('-p', '--port', default=DEFAULT_SERIAL_PORT,
              help="Serial port to connect to.")
@click.option('-b', '--baudrate', default=DEFAULT_BAUDRATE,
              help="Serial port baud rate.")
@click.option('-i', '--interval', default=DEFAULT_LEDS_STATE_INTERVAL_MS,
              help="Interval in milliseconds of LEDs state transmission.")
def main(port, baudrate, interval):
    serial_port = None
    leds_state_interval = float(interval) / 1000.0  # secs
    try:
        serial_port = serial.Serial(port, baudrate=baudrate)
        with serial.threaded.ReaderThread(
                serial_port, DeviceBSerialProtocol) as serial_protocol:
            assert isinstance(serial_protocol, DeviceBSerialProtocol)
            while True:
                serial_protocol.wait()
                if serial_protocol.should_send_leds_state():
                    leds_state = generate_leds_state_text()
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
