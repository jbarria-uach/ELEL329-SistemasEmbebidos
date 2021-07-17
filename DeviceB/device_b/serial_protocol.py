import threading
import serial.threaded


class DeviceBSerialProtocol(serial.threaded.LineReader):
    """Serial protocol handler class.

    It handles all the serial communications with the Device A from a thread.
    The protocol is built as:
    (Header)-(Command)-(arguments, optional)

    The header is always ELEL329.

    Right now Device B expects to receive: the following commands:
    * PRINT: To print the arguments string on the console.
    * REQLEDS: To start/stop the LEDS command transmission.

    And send the following commands:
    * LEDS: To specify the state of three LEDs (these are generated
            randomly for this project).

    """
    def __init__(self):
        """Constructor method"""
        self.__reqleds_on: bool = False
        self.__notifier: threading.Event = threading.Event()
        super().__init__()

    def connection_made(self, transport):
        """Callback method for 'connection made' events.

        Override of the parent's method with the objective of printing a
        message on the console when the connection is OK. It calls the
        parent's method after printing anyways.

        :param transport: The serial port instance. Not used by this override.
        """
        print("Serial port connection made")
        super().connection_made(transport)

    def connection_lost(self, exc):
        """Callback method for 'connection lost' events.

        Override of the parent's method with the objective of printing a
        message on the console when the connection is lost. It calls the
        parent's method after printing anyways.

        :param exc: An exception if the serial port raised any. 'None'
                    otherwise.
        """
        print("Serial port connection lost")
        if exc:
            print(exc)
            raise
        super().connection_lost(exc)

    def handle_line(self, line: str):
        """Handles a serial port received line.

        It checks if the string is compliant with the protocol. If so, handles
        the received command.

        :param line: the received string.
        """
        if line.startswith('ELEL329'):
            try:
                _, cmd, arg = line.split('-', maxsplit=2)
                if cmd == 'REQLEDS':
                    self._handle_reqleds_cmd(arg)
                elif cmd == 'PRINT':
                    self._handle_print_cmd(arg)
                else:
                    pass  # ignore invalid command
            except ValueError:
                pass  # ignore invalid package

    def should_send_leds_state(self) -> bool:
        """True if the LEDs state should be sent.

        It will be True after a REQLEDS-ON command is received.

        :return: True if a REQLEDS-ON has been received recently. False if a
                 REQLEDS-OFF has been received recently. False otherwise.
        """
        return self.__reqleds_on

    def wait(self):
        """Waits until a REQLEDS-ON command is received.

        It is expected to be called from the main thread to react after a
        REQLEDS-ON is received.
        """
        self.__notifier.wait()

    def _handle_reqleds_cmd(self, arg: str):
        """Handles a REQLEDS command.

        It sets the "notifier" object (to unblock the "wait()" method call)
        when a REQLEDS-OFF command is received.

        :param arg: The command argument.
        """
        if arg == 'ON':
            if not self.__reqleds_on:
                self.__reqleds_on = True
                self.__notifier.set()
                print("Device A requested to start LEDs state transmission")
        elif arg == 'OFF':
            if self.__reqleds_on:
                self.__reqleds_on = False
                self.__notifier.clear()
                print("Device A requested to stop LEDs state transmission")
        else:
            pass  # ignore invalid command

    @staticmethod
    def _handle_print_cmd(arg: str):
        """Handles a PRINT command.

        It prints the argument on the console.

        :param arg: The command argument.
        """
        print(f"Device A said: {arg}")
