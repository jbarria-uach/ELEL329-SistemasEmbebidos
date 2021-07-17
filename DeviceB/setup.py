from setuptools import setup

setup(
    name='device-b',
    version='0.2',
    packages=['device_b'],
    url='https://github.com/jbarria-uach/ELEL329-SistemasEmbebidos',
    license='MIT',
    author='Jorge Barría',
    author_email='jebarria@alumnos.uach.cl',
    description='ELEL329 Embedded Systems - Device B',
    install_requires=['click', 'pyserial'],
    entry_points={'console_scripts': ["dev_b = device_b:main"]}
)
