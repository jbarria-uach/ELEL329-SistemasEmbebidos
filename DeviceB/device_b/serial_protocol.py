import threading
import serial.threaded


class DeviceBSerialProtocol(serial.threaded.LineReader):
    def __init__(self):
        self.__reqleds_on: bool = False
        self.__notifier: threading.Event = threading.Event()
        super().__init__()

    def connection_made(self, transport):
        print("Serial port connection made")
        super().connection_made(transport)

    def connection_lost(self, exc):
        print("Serial port connection lost")
        if exc:
            print(exc)
            raise
        super().connection_lost(exc)

    def handle_line(self, line: str):
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
        return self.__reqleds_on

    def wait(self):
        self.__notifier.wait()

    def _handle_reqleds_cmd(self, arg: str):
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
        print(f"Device A said: {arg}")
