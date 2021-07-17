import random
import time

import click as click
import serial
import serial.threaded

from .serial_protocol import DeviceBSerialProtocol

# Default parameters.
DEFAULT_SERIAL_PORT = '/dev/ttyACM0'
DEFAULT_BAUDRATE = 115200
DEFAULT_LEDS_STATE_INTERVAL_MS = 250


def generate_leds_state_text() -> str:
    """Generates a string specifying random states for three LEDs.

    It generates three random states for three LEDs in "Device A" and
    creates a protocol compliant string to send the command.

    The protocol is built as:

    ELEL329-LEDS-HHL
      [1]   [2]  [3]
    Where:

    * [1] is the protocol header.
    * [2] is the command. The 'LEDS' command for this case.
    * [3] is the command argument. The three LEDs state in this case.
      'H' for 'high'(a.k.a. "on") state and 'L' for 'low' (a.k.a. "off")
      state.

    :return: the generated string.
    """
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
    """
    Main function.

    Main entry point of the program. Its arguments are parsed from CLI
    using the "click" module.

    :param port: The serial port to work with.
    :param baudrate: The serial port baud rate.
    :param interval: The interval of the LEDs state transmission (when
                     requested, defaults to 250ms for this project
                     requirements).
    """
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
