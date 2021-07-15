from setuptools import setup

setup(
    name='device-b',
    version='0.1',
    packages=['device_b'],
    url='',
    license='MIT',
    author='Jorge Barría',
    author_email='jebarria@alumnos.uach.cl',
    description='ELEL329 Embedded Systems - Device B',
    install_requires=['click'],
    entry_points={'console_scripts': ["dev_b = device_b:main"]}
)
